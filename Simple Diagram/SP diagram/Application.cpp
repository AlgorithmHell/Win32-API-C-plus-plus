#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES


#include <Windows.h>
#include "resource.h"
#include <iostream>
#include "Dialog.h"
#include <CommCtrl.h>
#include <Commdlg.h>
#include <map>
#include <vector>
#include <string>
#include<algorithm>
#include <functional>
#include <math.h>
using namespace std;

namespace Colors
{
	vector<COLORREF> _createdColors;
	COLORREF FromHex(int hex)
	{
		auto r = (hex & 0xFF0000) >> 16;
		auto g = (hex & 0x00FF00) >> 8;
		auto b = (hex & 0x0000FF) >> 0;

		auto rgb = RGB(r, g, b);

		_createdColors.push_back(rgb);

		return rgb;
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
const int amount = 6;
vector<pair<pair<int, double>, COLORREF> > MyItems;
vector<wstring> MyNames;


HINSTANCE hInst;
Dialog dialog;
MSG msg;
HDC hdc, hDC;
HBRUSH hNew, hOld, Brush;
HMENU hMenu;
HINSTANCE hInstance;


double d=3,D = 4;  
int x1, y11, x2, y2; 
int i;     
static short xw, yw;
double  r;
wstring mousePositionMessage;
BOOL IsExtended = false;









static void OnInitialize();
void SetDialogProperties();
void OnPaint();
void OnExtended();
void OnCommand(WPARAM wParam, HINSTANCE hInstance);
void OnCommandEdit(WPARAM wParam, HWND hWnd);

HWND GetControl(int id);
static void InitColorBox(HWND hDlg,int);

double recount(int val);
INT_PTR CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{


	hInst = hInstance;
	if (!CreateDialogParamW(hInstance, MAKEINTRESOURCE(IDD_MyDialog), nullptr, DialogProc, 0))
	{
		return FALSE;
	}

	ShowWindow((HWND)dialog, nCmdShow);


	while (GetMessage(&msg, nullptr, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}



	return (int)msg.wParam;
}

static INT_PTR CALLBACK DialogProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	

	switch (message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
		hMenu = GetMenu(hDialog);
		hMenu = GetSubMenu(hMenu, 0);
		break;
	case WM_INITDIALOG:
		dialog.Initialize(hDialog);
		hDC = GetDC(hDialog);
		OnInitialize();
		return (INT_PTR)TRUE;
	case WM_MOUSEMOVE:
		
	{
		if (IsExtended)
		{
			mousePositionMessage.clear();
			auto x = LOWORD(lParam);
			auto y = HIWORD(lParam);
			RECT Myrect;
			GetClientRect(hDialog, &Myrect);
			Myrect.left = Myrect.right - 250;
			Myrect.top = Myrect.bottom - 60;
			auto a = GetPixel(hDC, x, y);
			int k;
			for (k = 0; k <= 5; k++)
			{

				if (a == MyItems[k].second)
				{
					break;
				}
			}
			if (k != 6)
			{
				mousePositionMessage.append(MyNames[k]).append(L" ,").append(to_wstring(MyItems[k].first.first)).append(L" %");
			}

			InvalidateRect(hDialog, &Myrect, false);
		}
	}
		break;
	case WM_CLOSE:
		ReleaseDC(dialog, hDC);
		dialog.Destroy();
		return TRUE;

	case WM_COMMAND:
		OnCommand(wParam,hInstance);
		InvalidateRect(hDialog, 0, false);
		break;
	case WM_PAINT:
		OnPaint();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	
	case WM_COMMAND:
	{

		OnCommandEdit(wParam, hWnd);
	}
	break;




	case WM_CLOSE:
	{
		DeleteObject(Brush);
		EndDialog(hWnd, 0);
	}
	return TRUE;



	}
	return (INT_PTR)FALSE;
}
static void OnInitialize()
{

	MyItems.push_back(make_pair(make_pair(20, recount(20)), Colors::Firebrick));
	MyItems.push_back(make_pair(make_pair(10, recount(10)), Colors::Turquoise3));
	MyItems.push_back(make_pair(make_pair(18, recount(18)), Colors::Yellow));
	MyItems.push_back(make_pair(make_pair(12, recount(12)), Colors::SpringGreen3));
	MyItems.push_back(make_pair(make_pair(16, recount(16)), Colors::SteelBlue2));
	MyItems.push_back(make_pair(make_pair(24, recount(24)), Colors::Goldenrod1));
	MyNames.push_back(L"Rachel Wizard");
	MyNames.push_back(L"Donald Tone");
	MyNames.push_back(L"Carry Bradshow");
	MyNames.push_back(L"MIchael Ring");
	MyNames.push_back(L"John Adams");
	MyNames.push_back(L"Benedict Hand");

	MyItems.resize(amount);
	MyNames.resize(amount);


}
void SetDialogProperties()
{

	dialog.SetCaption(L" ");
}
void OnPaint()
{
	hdc = dialog.BeginPaint();
	RECT Myrect;
	GetClientRect(dialog, &Myrect);
	Myrect.left = Myrect.right - 220;
	Myrect.top = Myrect.bottom - 20;
	RECT rect = dialog.GetClientRect();
	xw = (rect.right - rect.left) / 2 + 200;
	yw = (rect.bottom - rect.top) / 2;
	r = yw < xw ? (double)(yw / D*d) : (double)(xw / D*d);
	SetTextAlign(hdc, TA_CENTER);   // Выравнивание текста
	SetBkMode(hdc, TRANSPARENT); // Цвет фона текста
	TextOut(hdc, xw, 30, L"RESULTS", 8);
	SelectObject(hdc, CreateSolidBrush(Colors::White));
	Rectangle(hdc, Myrect.left, Myrect.top, Myrect.right, Myrect.bottom);
	double calc = 0;
	double calc2 = (double)MyItems[0].first.second;
	for (i = 0; i <= amount - 1; i++) // Цикл по количеству «долек пирога»
	{
		hNew = CreateSolidBrush(MyItems[i].second);
		hOld = (HBRUSH)SelectObject(hdc, hNew);

		x1 = (int)(xw + r*cos(2 * M_PI*calc / 360));
		y11 = (int)(yw - r*sin(2 * M_PI*calc / 360));
		x2 = (int)(xw + r*cos(2 * M_PI*calc2 / 360));
		y2 = (int)(yw - r*sin(2 * M_PI*calc2 / 360));
		Pie(hdc, xw - r, yw - r, xw + r, yw + r, x1, y11, x2, y2);
		

		
		if (i != 5)
		{
			calc += MyItems[i].first.second;
			calc2 += MyItems[i + 1].first.second;
		}
		TextOut(hdc, (x1 + x2) / 2 + (i >= 3 ? 15 : -5), (y11 + y2) / 2 + (i >= 3 ? -10 : 10), (to_wstring(MyItems[i].first.first) + L" %").c_str(), std::to_string(MyItems[i].first.first).size() + 3);
		DeleteObject(hNew);
	}

	MoveToEx(hdc, rect.left + 100, rect.top + 40, 0);
	LineTo(hdc, rect.left + 100, rect.bottom - 20);
	LineTo(hdc, rect.left + 430, rect.bottom - 20);
	int MaxHeight = rect.top + 40;
	int MinHeight = rect.bottom - 20;
	int MaxWidth = rect.left + 430;
	int MinWidth = rect.left + 130;
	int per = 20;
	POINT point;
	for (int i = 1; i <= 5; i++)
	{

		point.x = rect.left + 75;
		point.y = rect.bottom - 20 - i*(rect.bottom - 70) / 5;
		MoveToEx(hdc, rect.left + 95, point.y, 0);
		LineTo(hdc, rect.left + 105, point.y);
		TextOut(hdc, point.x, point.y, (to_wstring(per) + L" %").c_str(), std::to_string(20).size() + 3);
		per += 20;
	}
	int dx = 0;
	for (int i = 0; i <= 5; i++)
	{
		dx += 30;
		hNew = CreateSolidBrush(MyItems[i].second);
		hOld = (HBRUSH)SelectObject(hdc, hNew);
		Rectangle(hdc, MinWidth + dx, MinHeight - (MinHeight - MaxHeight)*MyItems[i].first.first / 100 * 4, MinWidth + dx + 30, MinHeight);

		DeleteObject(hNew);
	}
	if(IsExtended)
	DrawText(hdc, mousePositionMessage.c_str(), -1, &rect, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
	
	dialog.EndPaint();
}
void OnExtended()
{
	if (GetMenuState(hMenu, ID_ExtendedView, MF_BYCOMMAND) == MF_UNCHECKED)
	{
		CheckMenuItem(hMenu, ID_ExtendedView, MF_BYCOMMAND | MF_CHECKED);
		IsExtended = true;
	}
	if (GetMenuState(hMenu, ID_ExtendedView, MF_BYCOMMAND) == MF_CHECKED)
	{
		CheckMenuItem(hMenu, ID_ExtendedView, MF_BYCOMMAND | MF_UNCHECKED);
		IsExtended = false;
	}
}
void OnCommand(WPARAM wParam, HINSTANCE hInstance)
{
	switch (LOWORD(wParam))
	{
	case ID_Colors:
	{
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_EditDialog), dialog, EditProc);
		break;
	}
	case ID_ExtendedView:
	{
		OnExtended();
		IsExtended = !IsExtended;
		break;
	}

	}


}
void OnCommandEdit(WPARAM wParam, HWND hWnd)

{
	switch (LOWORD(wParam))
	{
	case IDC_1:
	{
		InitColorBox(hWnd, 0);
		break;
	}
	case IDC_2:
	{
		InitColorBox(hWnd, 1);
		break;
	}
	case IDC_3:
	{
		InitColorBox(hWnd, 2);
		break;
	}
	case IDC_4:
	{
		InitColorBox(hWnd, 3);
		break;
	}
	case IDC_5:
	{
		InitColorBox(hWnd, 4);
		break;
	}
	case IDC_6:
	{
		InitColorBox(hWnd, 5);
		break;
	}
	case IDC_view:
	{
		if (IsExtended)
		{
			IsExtended = !IsExtended;
			
			SendMessage(GetControl(IDC_Status), WM_SETTEXT, 0, (LPARAM)L"Status: Enabled");
			EndDialog(hWnd,0);
		}
		else
		{
			IsExtended = !IsExtended;
			SendMessage(GetControl(IDC_Status), WM_SETTEXT, 0, (LPARAM)L"Status: Disabled");
			EndDialog(hWnd, 0);
		}
		break;
	}
	
	}
}
HWND GetControl(int id)
{
	return GetDlgItem(dialog, id);
}
static void InitColorBox(HWND hDlg,int index)
{
	
	COLORREF chosenColor;

	CHOOSECOLOR cc;
	COLORREF *acrCustClr = new COLORREF[Colors::_createdColors.size()];

	for (unsigned i = 0; i < Colors::_createdColors.size(); ++i)
	{
		acrCustClr[i] = Colors::_createdColors.at(i);
	}

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = NULL;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE)
	{
		chosenColor = cc.rgbResult;
		Brush = CreateSolidBrush(chosenColor);
		MyItems[index].second = chosenColor;
	}
	else
	{
		chosenColor = NULL;
	}
}

double recount(int val)
{
	return (double)val * 360 / 100;
}
