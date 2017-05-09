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

	const COLORREF Black = FromHex(0x000000);
	const COLORREF White = FromHex(0xFFFFFF);
	const COLORREF Red = FromHex(0xf45151);
	const COLORREF Orange = FromHex(0xf4c451);
	const COLORREF Yellow = FromHex(0xf3f451);
	const COLORREF BrightYellow = FromHex(0xffd700);
	const COLORREF Green = FromHex(0x57f451);
	const COLORREF BrightGreen = FromHex(0x1acc41);
	const COLORREF LightBlue = FromHex(0x51f4f0);
	const COLORREF Blue = FromHex(0x546aab);
	const COLORREF Purple = FromHex(0xca51f4);
	const COLORREF Brown = FromHex(0x836828);
	const COLORREF Strawberry = FromHex(0xCE648E);
	const COLORREF Turquoise = FromHex(0x40E0D0);
	const COLORREF Lime = FromHex(0xc6fb04);
	const COLORREF LightGreen = FromHex(0x8bba90);
	const COLORREF OrangeRed = FromHex(0xFF4500);
	const COLORREF DeepSkyBlue = FromHex(0x00BFFF);
}


HINSTANCE hInst;
Dialog dialog;
MSG msg;
HDC MyDC, hDC;
HBRUSH hNew, hOld, Brush;
HMENU hMenu;
HINSTANCE hInstance;
int ChangedHeight=1000, ChangedWidth=1000;
RECT rect;
HBRUSH YellowBrush, BlueBrush, StrawBerryBrush, OrangeBrush;
const int SCALE = 1, TRANSLATE = 2, ROTATE = 3, SHEAR = 4, REFLECT = 5, NORMAL = 6;
vector<POINT> coordinates;
RECT ButtonRect;
 int Radius =300;
 const int coefficient = 100;
 int OffsetX = 0;
 int OffsetY = 0;
 static int ticks = 0;
 int radius = 50;
 int start = 0;
 double Count = 0.0;
 int cx, cy;
 POINT dot;
 HWND UP, DOWN, LEFT, RIGHT;
 const int ID_Up=1002, ID_Down=1003, ID_Left=1004, ID_Right = 1005;
static void OnInitialize(HWND);
void SetDialogProperties();
void OnPaint(HWND&);
void CreateBrushes();
void DeleteBrushes();
void CALLBACK TimerProc(HWND hWnd, UINT _, UINT_PTR __, DWORD ms);

void UpdateDot(time_t dt);
void OnCommand(WPARAM wParam, HINSTANCE hInstance, HWND);
void OnCommandEdit(WPARAM wParam, HINSTANCE hInstance, HWND hWnd);
void SendImage(const wchar_t*caption, HWND& control);
HWND GetControl(int id);
void TransformAndDraw(int iTransform, HWND hWnd);
HWND CreateControl(HWND&,const wchar_t* className, long style, int x, int y, int width, int heigth, int id);

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

static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
	case WM_SIZE:
	{
		
		 cx = LOWORD(lParam);
		 cy = HIWORD(lParam);
		MoveWindow(UP, cx - 120, 10, 50, 50, 1);
		MoveWindow(DOWN, cx - 120, 70, 50, 50, 1);
		MoveWindow(LEFT, cx - 180,  70, 50, 50, 1);
		MoveWindow(RIGHT, cx - 60,  70, 50, 50, 1);
		cx /= 2;
		cy /= 2;
		InvalidateRect(UP,0,false);
		InvalidateRect(DOWN, 0, false);
		InvalidateRect(LEFT, 0, false);
		InvalidateRect(RIGHT, 0, false);
		break;
	}
	case WM_CREATE:
		CreateBrushes();

		break;
	case WM_INITDIALOG:
		dialog.Initialize(hWnd);
		SetTimer(hWnd, 1, 10, TimerProc); 
		OnInitialize(hWnd);
		return (INT_PTR)TRUE;
	
	case WM_CLOSE:
		DeleteBrushes();
		dialog.Destroy();
		return TRUE;
	
	case WM_COMMAND:
		OnCommand(wParam, hInstance, hWnd);
		
		break;
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	case WM_DESTROY:
		DeleteBrushes();
		PostQuitMessage(0);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}

 void OnInitialize(HWND hWnd)
{
	
	
	UP = CreateControl(hWnd,WC_BUTTON, BS_BITMAP, 650, 10, 50, 50, ID_Up);
	DOWN = CreateControl(hWnd, WC_BUTTON, BS_BITMAP, 650, 70, 50, 50, ID_Down);
	LEFT = CreateControl(hWnd, WC_BUTTON, BS_BITMAP, 590, 70, 50, 50, ID_Left);
	RIGHT = CreateControl(hWnd, WC_BUTTON, BS_BITMAP, 710, 70, 50, 50, ID_Right);

	
	SendImage(L"down.bmp", DOWN);
	SendImage(L"up.bmp", UP);
	SendImage(L"left.bmp", LEFT);
	SendImage(L"right.bmp", RIGHT);
	
	

}
void SetDialogProperties()
{

	dialog.SetCaption(L" ");
}
void CreateBrushes()
{
	YellowBrush = CreateSolidBrush(Colors::BrightYellow);
	BlueBrush = CreateSolidBrush(Colors::DeepSkyBlue);
	StrawBerryBrush = CreateSolidBrush(Colors::Strawberry);
	OrangeBrush = CreateSolidBrush(Colors::OrangeRed);
}
void DeleteBrushes()
{
	DeleteObject(YellowBrush);
	DeleteObject(BlueBrush);
	DeleteObject(StrawBerryBrush);
	DeleteObject(OrangeBrush);
}
void CALLBACK TimerProc(HWND hWnd, UINT _, UINT_PTR __, DWORD ms)
{
	auto dt = ms - start;
	start = ms;

	UpdateDot(dt);

	InvalidateRect(hWnd, NULL, FALSE);
}

void UpdateDot(time_t dt)
{
	if (Count > -2 * M_PI)
	{
		Count -= 0.001 * dt;
		dot.x = (LONG)(OffsetX+ Radius *(1 - cos(Count)) * cos(Count) );
		dot.y = (LONG)(OffsetY+Radius *(1 - cos(Count)) * sin(Count) );
	}
	else
	{
		Count -= 0.001 * dt;
		dot.x = (LONG)(OffsetX+ Radius * (1 - cos(Count)) * cos(Count) );
		dot.y = (LONG)(OffsetY+ Radius * (1 - cos(Count)) * sin(Count) );
		if (Count < -4 * M_PI)
			Count = 0.0;

	}
}
void OnPaint(HWND &hWnd)
{
	coordinates.clear();
	MyDC = dialog.BeginPaint();

	GetClientRect(dialog, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	
	auto hdc = CreateCompatibleDC(MyDC);
	auto CompBitmap = CreateCompatibleBitmap(MyDC, width, height);
	auto OldBitmap = (HBITMAP)SelectObject(hdc, CompBitmap);
	PatBlt(hdc, 0, 0, width, height, WHITENESS);

	
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, 1000, 1000, nullptr);
	SetViewportExtEx(hdc, (rect.right - rect.left) / 2, -(rect.bottom - rect.top) / 2, 0);
	SetViewportOrgEx(hdc, (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2, 0);
	MoveToEx(hdc, -1000, 0, 0);
	LineTo(hdc, 1000, 0);
	MoveToEx(hdc, 0, 1000, 0);
	LineTo(hdc, 0, -1000);
	for (int i = 0, a = -10; i <= 20; i++, a++)
	{
		MoveToEx(hdc, -1000 + coefficient*i, -10, 0);
		LineTo(hdc, -1000 + coefficient*i, 10);
		MoveToEx(hdc, -10, -1000 + coefficient*i, 0);
		LineTo(hdc, 10, -1000 + coefficient*i);
		if (a != 0)
		{
			TextOut(hdc, a > 0 ? -1000 + coefficient*i - 30 : -1000 + coefficient*i + 10, -20, (LPCWSTR)to_wstring(a).c_str(), to_wstring(a).size());

			TextOut(hdc, -40, a < 0 ? -1000 + coefficient*i + 45 : -1000 + coefficient*i + 10, (LPCWSTR)to_wstring(a).c_str(), to_wstring(a).size());
		}
	}
	
	//POINT apt[130];
	int i = 0;
	MoveToEx(hdc, OffsetX, OffsetY, 0);
	for (double angle = 0.0; angle <2 * M_PI; angle += 0.05)
	{
		POINT p;

		p.x = Radius*(1 - cos(angle))*cos(angle) + OffsetX;
		p.y = Radius*(1 - cos(angle))*sin(angle) + OffsetY;
		coordinates.push_back(p);
		//apt[i] = p;
		i++;
		
			LineTo(hdc, p.x, p.y);


	}
	//Polyline(hdc, apt, i);

	SelectObject(hdc, CreateSolidBrush(Colors::Strawberry));
	if (ticks >= coordinates.size()) {
		int i = ticks;
		++i;
	}
	Ellipse(hdc, dot.x - radius, dot.y - radius, dot.x + radius, dot.y + radius);
	
	SetMapMode(hdc, MM_TEXT);
	SetWindowExtEx(hdc, (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2, nullptr);
	SetViewportExtEx(hdc, 0, 0, 0);
	SetViewportOrgEx(hdc, 0, 0, 0);

	BitBlt(MyDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	DeleteObject(CompBitmap);
	DeleteDC(hdc);
	
	
	dialog.EndPaint();


	
}

HWND CreateControl(HWND &parent, const wchar_t* className, long style, int x, int y, int width, int heigth, int id)
{
return CreateWindow(
className, L"",
WS_CHILD | WS_VISIBLE | style,
x, y,
width, heigth,
parent,
(HMENU)id,
hInst,
nullptr);
}
void OnCommand(WPARAM wParam, HINSTANCE hInstance, HWND hWnd)
{
	switch (LOWORD(wParam))
	{

	case ID_Up:
		OffsetY += 50;
		
		break;
	case ID_Down:
		OffsetY -= 50;
		
		break;
	case ID_Right:
		OffsetX += 50;
		
		break;
	case ID_Left:
		OffsetX -= 50;
		
		break;


	}


}
void OnCommandEdit(WPARAM wParam, HINSTANCE hInstance, HWND hWnd)
{
	switch (LOWORD(wParam))
	{



	}


}

HWND GetControl(int id)
{
	return GetDlgItem(dialog, id);
}

void TransformAndDraw(int iTransform, HWND hWnd)
{
	HDC hDC;
	XFORM xForm;
	RECT rect;

	// Retrieve a DC handle for the application's window.  

	hDC = GetDC(hWnd);

	// Set the mapping mode to LOENGLISH. This moves the  
	// client area origin from the upper left corner of the  
	// window to the lower left corner (this also reorients  
	// the y-axis so that drawing operations occur in a true  
	// Cartesian space). It guarantees portability so that  
	// the object drawn retains its dimensions on any display.  

	SetGraphicsMode(hDC, GM_ADVANCED);
	SetMapMode(hDC, MM_LOENGLISH);

	// Set the appropriate world transformation (based on the  
	// user's menu selection).  
	GetClientRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	switch (iTransform)
	{
	case SCALE:        // Scale to 1/2 of the original size.  
		xForm.eM11 = (FLOAT) 1.0;
		xForm.eM12 = (FLOAT) 0.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT) -1.0;
		xForm.eDx = (FLOAT) width;
		xForm.eDy = (FLOAT) height;
		SetWorldTransform(hDC, &xForm);
		break;

	case TRANSLATE:   // Translate right by 3/4 inch.  
		xForm.eM11 = (FLOAT) 1.0;
		xForm.eM12 = (FLOAT) 0.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT) 1.0;
		xForm.eDx = (FLOAT) 75.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	case ROTATE:      // Rotate 30 degrees Counterclockwise.  
		xForm.eM11 = (FLOAT) 0.8660;
		xForm.eM12 = (FLOAT) 0.5000;
		xForm.eM21 = (FLOAT)-0.5000;
		xForm.eM22 = (FLOAT) 0.8660;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	case SHEAR:       // Shear along the x-axis with a  
					  // proportionality constant of 1.0.  
		xForm.eM11 = (FLOAT) 1.0;
		xForm.eM12 = (FLOAT) 1.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT) 1.0;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	case REFLECT:     // Reflect about a horizontal axis.  
		xForm.eM11 = (FLOAT) 1.0;
		xForm.eM12 = (FLOAT) 0.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT)-1.0;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	case NORMAL:      // Set the unity transformation.  
		xForm.eM11 = (FLOAT) 0.0;
		xForm.eM12 = (FLOAT) 0.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT) 0.0;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	}

	// Find the midpoint of the client area.  

	GetClientRect(hWnd, (LPRECT)&rect);
	DPtoLP(hDC, (LPPOINT)&rect, 2);

	// Select a hollow brush.  

	SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));



	// Draw the key.  

	Rectangle(hDC, 0, 0,-100, 100);


	

	ReleaseDC(hWnd, hDC);
}

void SendImage(const wchar_t*caption,HWND&control)
{
	auto bmp = (HBITMAP)LoadImage(nullptr, caption, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);
	SendMessage(control, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmp);
	//UpdateWindow(control);
}
