#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "resource.h"
#include "Dialog.h"
#include <string>

#define PLAIN_RECT(rect) rect.left,rect.top,rect.right,rect.bottom
enum {RED=1,YELLOW,GREEN};
const int TICKS = 3;
namespace Colors
{
	static COLORREF FromHex(int hex)
	{
		auto r = (hex & 0xFF0000) >> 16;
		auto g = (hex & 0x00FF00) >> 8;
		auto b = (hex & 0x0000FF) >> 0;
		return RGB(r, g, b);
	}

	static const COLORREF Firebrick = FromHex(0xB22222);
	static const COLORREF Goldenrod1 = FromHex(0xFFC125);
	static const COLORREF SpringGreen3 = FromHex(0x00CD66);
	static const COLORREF Black = FromHex(0x000000);
	static const COLORREF White = FromHex(0xFFFFFF);
	static const COLORREF SteelBlue2 = FromHex(0x5CACEE);
	static const COLORREF SlateBlue = FromHex(0x6A5ACD);
	static const COLORREF Sienna1 = FromHex(0xFF8247);
	static const COLORREF Turquoise3 = FromHex(0x00C5CD);
	static const COLORREF DarkRed = FromHex(0x8b0000);
	static const COLORREF DarkYellow = FromHex(0xb3a700);
	static const COLORREF DarkGreen = FromHex(0x006400);
	static const COLORREF Yellow = FromHex(0xffee00);
}

class Application
{
	static Dialog MyDialog;
	static const int MinDialogWidth = 400;
	static const int MinDialogHeight =300;
	static const int TimerID = 1;
	
	

	static int timerticks;
	static int switcher;

public:
	static int Main(HINSTANCE hInstance, int nCmdShow)
	{
		MyDialog.Create(hInstance, IDD_MAINDIALOG, DialogProc);
		MyDialog.Show(nCmdShow);

		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!IsDialogMessage(MyDialog, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return (int)msg.wParam;
	}

private:
	static INT_PTR CALLBACK DialogProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_INITDIALOG:
			MyDialog.Initialize(hDialog);
			OnInitialize();
			return (INT_PTR)TRUE;

		case WM_CLOSE:
			MyDialog.Destroy();
			return TRUE;

		case WM_TIMER:
		{
			auto timerID = (int)wParam;
			if (timerID == TimerID)
			{
				--timerticks;
				if (timerticks ==0)
				{
					switcher++;
					if (switcher == 5)
					{
						switcher = 1;
					}
					timerticks = TICKS;
				}
				MyDialog.InvalidateRect();
			}
			
					
			return TRUE;
		}

		case WM_PAINT:
			OnPaint();
			
			return TRUE;

		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;

		case WM_SIZE:
			MyDialog.InvalidateRect();
			return TRUE;

		case WM_GETMINMAXINFO:
		{
			auto sizeInfo = (LPMINMAXINFO)lParam;
			sizeInfo->ptMinTrackSize.x = MinDialogWidth;
			sizeInfo->ptMinTrackSize.y = MinDialogHeight;
			return TRUE;
		}
		}
		return (INT_PTR)FALSE;
	}

	static void OnInitialize()
	{
		MyDialog.SetSize(MinDialogWidth, MinDialogHeight);
		MyDialog.SetCaption(L"Traffic Lights");

		SetTimer(MyDialog, TimerID, 1000, nullptr);
		
		
	}


	static void OnPaint()
	{
		auto hDeviceContext = MyDialog.BeginPaint();

		auto rect = MyDialog.GetClientRect();
		auto width = rect.right - rect.left;
		auto height = rect.bottom - rect.top;
		auto unit = min(width, height) / 2;
		

		// Lines.
		SelectAndReleaseObject(hDeviceContext, CreatePen(PS_DASHDOTDOT, 1, Colors::SteelBlue2));
		for (int i = 1; i < 6; i++)
		{
			MoveToEx(hDeviceContext, 0, i*height / 6, nullptr);
			LineTo(hDeviceContext, width, i*height / 6);
		}		
		MoveToEx(hDeviceContext, width / 2, 0, nullptr);
		LineTo(hDeviceContext, width / 2, height);

		if (switcher == 1) 
		{
				
				Redraw(hDeviceContext, rect, width, height, unit,RGB(255,0,0),Colors::DarkYellow,Colors::DarkGreen,RED);
					
		}
		if (switcher == 2||switcher==4)
		{
			
				Redraw(hDeviceContext, rect, width, height, unit,Colors::DarkRed,Colors::Yellow,Colors::DarkGreen,YELLOW);
							
			
		}
		if (switcher == 3)
		{
			
				Redraw(hDeviceContext, rect, width, height, unit,Colors::DarkRed,Colors::DarkYellow,RGB(0,255,0),GREEN);
	
			
		}
		

		
		MyDialog.ValidateRect();
		MyDialog.EndPaint();
	}

	static RECT CalculateShapeRect(int xCenter, int yCenter, double width, double heigth)
	{
		auto x = xCenter - (int)width / 2;
		auto y = yCenter - (int)heigth / 2;
		return CreateRect(x, y, x + (int)width, y + (int)heigth);
	}

	static RECT CreateRect(int left, int top, int rigth, int bottom)
	{
		auto rect = RECT();
		rect.left = left;
		rect.top = top;
		rect.right = rigth;
		rect.bottom = bottom;
		return rect;
	}

	static HFONT CreateFont(const wchar_t* name, int size, int weight)
	{
		auto lf = LOGFONT{};
		wcscpy_s(lf.lfFaceName, name);
		lf.lfHeight = size;
		lf.lfWeight = weight;
		lf.lfCharSet = DEFAULT_CHARSET;
		return CreateFontIndirect(&lf);
	}

	

	static void SelectAndReleaseObject(HDC hDeviceContext, HGDIOBJ hGdiObject)
	{
		auto hOldGdiObject = SelectObject(hDeviceContext, hGdiObject);
		DeleteObject(hOldGdiObject);
	}



	
	static void DrawRound(HDC hDeviceContext,RECT rect)
	{
		Ellipse(hDeviceContext, PLAIN_RECT(rect));
	}
	static void Redraw(HDC hDeviceContext, RECT rect, int width, int height, int unit, COLORREF Red, COLORREF Yellow, COLORREF Green,int light_number )
	{

		if (light_number == 2)light_number++;
		else
		if (light_number == 3) light_number += 2;

		// Traffic lights
		SelectAndReleaseObject(hDeviceContext, CreateSolidBrush(Red));
		SelectAndReleaseObject(hDeviceContext, CreatePen(PS_SOLID, 1, Red));
		DrawRound(hDeviceContext, CalculateShapeRect(width / 2, unit / 3, 0.45*unit, 0.45*unit));
		SelectAndReleaseObject(hDeviceContext, CreateSolidBrush(Yellow));
		SelectAndReleaseObject(hDeviceContext, CreatePen(PS_SOLID, 1,Yellow));
		DrawRound(hDeviceContext, CalculateShapeRect(width / 2, unit * 3 / 3, 0.45*unit, 0.45*unit));
		SelectAndReleaseObject(hDeviceContext, CreateSolidBrush(Green));
		SelectAndReleaseObject(hDeviceContext, CreatePen(PS_SOLID, 1, Green));
		DrawRound(hDeviceContext, CalculateShapeRect(width / 2, unit * 5 / 3, 0.45*unit, 0.45*unit));



		//Timer
		auto hOldFont = SelectObject(hDeviceContext, CreateFont(L"Consolas", 0.3 * unit, FW_BOLD));
		SetTextColor(hDeviceContext, Colors::SlateBlue);
		SetBkMode(hDeviceContext, TRANSPARENT);
		auto textRectangle= CalculateShapeRect(width / 2, unit *light_number / 3, 0.45*unit, 0.45*unit);
		auto text = std::to_wstring(timerticks);
		DrawText(hDeviceContext, text.c_str(), -1, &textRectangle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);//впишет в пряомугольник и выровняеет его




		SelectAndReleaseObject(hDeviceContext, hOldFont);
	}
	
	
};

Dialog Application::MyDialog = Dialog();
int Application::timerticks = TICKS;
int Application::switcher = 1;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return Application::Main(hInstance, nCmdShow);
}