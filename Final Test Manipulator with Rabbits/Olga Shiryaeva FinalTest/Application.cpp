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
#include <ctime>
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
HDC hdc, hDC;
HBRUSH hNew, hOld, Brush;
HMENU hMenu;
HINSTANCE hInstance;
vector <vector <pair<BOOL,RECT> > > steps;
const int lines = 15, columns = 20;
static int counter = 0;
HBRUSH GreenBrush, BlackBrush, StrawBerryBrush, WhiteBrush;
vector<COLORREF> MyColors;
wstring Mytext=L"";
BOOL uVal;
bool gameEnd = false;
bool inputI = false;
bool inputJ = false;
bool flag1=false, flag2=false, flag3=false, flag4 = false;
int variableI = 0, variableJ = 0;
wstring buffer, otherbuffer;

static void OnInitialize(LPARAM lParam, HWND hWnd);
void SetDialogProperties();
void OnPaint();
void CreateBrushes();
void DeleteBrushes();
void OnCommand(WPARAM wParam, HINSTANCE hInstance, HWND);
void OnCommandEdit(WPARAM wParam, HINSTANCE hInstance, HWND hWnd);
void ProcessTimer(HWND hWnd,vector < vector <pair <BOOL, RECT> > > &steps);
void ProcessStep(HWND hWnd,vector < vector <pair <BOOL, RECT> > > &steps, int i, int j);

void GetText(int id, wstring&Text,HWND);
void OnMouseMove(LPARAM lParam);
void OnButtonDown(LPARAM lParam);
void UpDateFlags();

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

static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
	case WM_TIMER:
	{
		counter++;
		ProcessTimer(hWnd,steps);
		InvalidateRect(hWnd, 0, true);
		break;
	}
	case WM_MOUSEMOVE:
	{
		OnMouseMove(lParam);
		InvalidateRect(hWnd, 0, false);
		break;
	}
	case WM_CREATE:
		
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
		hMenu = GetMenu(hWnd);
		hMenu = GetSubMenu(hMenu, 0);
		break;
	case WM_INITDIALOG:
		SetTimer(hWnd, 1, 1000, 0);
		dialog.Initialize(hWnd);
		OnInitialize(lParam, hWnd);
		
		return (INT_PTR)TRUE;
	case WM_LBUTTONDOWN:
	{ 
		OnButtonDown(lParam);
		break;
	}
	case WM_CLOSE:
		DeleteBrushes();
		dialog.Destroy();
		return TRUE;

	case WM_COMMAND:
		OnCommand(wParam, hInstance, hWnd);
		InvalidateRect(hWnd, 0, false);
		break;
	case WM_PAINT:
		OnPaint();
		break;
	case WM_DESTROY:
		DeleteBrushes();
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

		OnCommandEdit(wParam, hInstance, hWnd);
	}
	break;




	case WM_CLOSE:
	{

		EndDialog(hWnd, 0);
	}
	return TRUE;



	}
	return (INT_PTR)FALSE;
}
static void OnInitialize(LPARAM lParam,HWND hWnd)
{
	CreateBrushes();
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom -= 40;
	
	int columnOffset = (rect.right - rect.left) / columns;
	int lineOffset = (rect.bottom - rect.top) / lines;
	steps.resize(columns);
	for (int i = 0; i < columns; i++)
	{
		steps[i].resize(lines);
		for (int j = 0; j < lines; j++)
		{
			RECT buf;
			buf.left = rect.left + i*columnOffset;
			buf.right = rect.left + (i + 1)*columnOffset;
			buf.top = rect.top + j*lineOffset;
			buf.bottom = rect.top + (j + 1)*lineOffset;
			steps[i][j].second = buf;
			steps[i][j].first = rand() % 2;
		}
	}
	srand(time(NULL));
	steps[rand()%20][rand()%15].first = 2;
	
	
}
void SetDialogProperties()
{

	dialog.SetCaption(L" ");
}
void CreateBrushes()
{
	GreenBrush = CreateSolidBrush(Colors::Green);
	BlackBrush = CreateSolidBrush(Colors::Black);
	StrawBerryBrush = CreateSolidBrush(Colors::Strawberry);
	WhiteBrush = CreateSolidBrush(Colors::White);
}
void DeleteBrushes()
{
	DeleteObject(GreenBrush);
	DeleteObject(BlackBrush);
	DeleteObject(StrawBerryBrush);
	DeleteObject(WhiteBrush);
}
void OnPaint()
{
	PAINTSTRUCT ps;
	RECT Myrect;
	GetClientRect(dialog, &Myrect);
	int width = Myrect.right - Myrect.left;
	int height = Myrect.bottom - Myrect.top;
	auto MyDC = BeginPaint(dialog, &ps);

	auto hdc = CreateCompatibleDC(MyDC);
	auto CompBitmap = CreateCompatibleBitmap(MyDC, width, height);
	auto OldBitmap = (HBITMAP)SelectObject(hdc, CompBitmap);
	PatBlt(hdc, 0, 0, width, height, WHITENESS);

	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < lines; j++)
		{
			SelectObject(hdc, steps[i][j].first == 0 ? BlackBrush : steps[i][j].first == 1 ? GreenBrush:WhiteBrush );
			if(steps[i][j].first!=2)
			Rectangle(hdc, steps[i][j].second.left, steps[i][j].second.top, steps[i][j].second.right, steps[i][j].second.bottom);
			else
			{
				SelectObject(hdc, GreenBrush);
				Rectangle(hdc, steps[i][j].second.left, steps[i][j].second.top, steps[i][j].second.right, steps[i][j].second.bottom);
				SelectObject(hdc, StrawBerryBrush);
				Ellipse(hdc, steps[i][j].second.left, steps[i][j].second.top, steps[i][j].second.right, steps[i][j].second.bottom);
			}
		}
	}
	if(!gameEnd)
	TextOut(hdc, Myrect.right-20, Myrect.bottom-20, (LPCWSTR)to_wstring(counter).c_str(), to_wstring(counter).size());


	BitBlt(MyDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	DeleteObject(CompBitmap);
	DeleteDC(hdc);

	EndPaint(dialog, &ps);
	
}
void OnCommand(WPARAM wParam, HINSTANCE hInstance, HWND hWnd)
{
	switch (LOWORD(wParam))
	{
	case ID_Edit:
	{
		KillTimer(hWnd, 1);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_EditDialog), hWnd, EditProc);
		SetTimer(hWnd, 1, 1000, 0);
		break;
	}
	case ID_CloseWindow:
	{
		DestroyWindow(hWnd);
		break;
	}

	}


}
void OnCommandEdit(WPARAM wParam, HINSTANCE hInstance, HWND hWnd)
{
	
	
	auto controlID = LOWORD(wParam);
	auto notification = HIWORD(wParam);

	switch (controlID)
	{
	case IDC_EDIT1:
		switch (notification)
		{
		case EN_CHANGE:
		{
			
			variableJ = GetDlgItemInt(hWnd, IDC_EDIT1, &uVal, 0);
			if (variableJ >= 20) MessageBox(hWnd, (LPCWSTR)(L"Incorrect input, try again"), (LPCWSTR)L"Input", MB_OK);
			else
			{
				inputJ = true;
				if (inputI) EnableWindow(GetDlgItem(hWnd, IDC_Confirm), 1);
			}
			break;
		}


		}
		break;
	case IDC_EDIT2:
		switch (notification)
		{
		case EN_CHANGE:
		{
			
			variableI = GetDlgItemInt(hWnd, IDC_EDIT2, &uVal, 0);
			
			if (variableI>=15) MessageBox(hWnd, (LPCWSTR)(L"Incorrect input, try again"), (LPCWSTR)L"Input", MB_OK);
			else
			{
				inputI = true;
				if (inputJ) EnableWindow(GetDlgItem(hWnd, IDC_Confirm), 1);
			}
			break;
		}
		
		}
		break;
	case IDC_Confirm:
	{

		if (inputI&&inputJ)
		{
			inputI = false;
			inputJ = false;
			steps[variableI][variableJ].first = 2;
			gameEnd = false;
			counter = 0;
			EndDialog(hWnd, 0);
		}
		break;
	}
	case IDC_Cancel:
	{
		EndDialog(hWnd, 0);
		break;
	}

	}
		


}
void GetText(int id,wstring&Text,HWND hWnd)
{
	
	auto textLength = GetWindowTextLengthW(GetDlgItem(hWnd,id));
	wstring buffer;
	buffer.resize(textLength + 1);
	//auto buffer = new wchar_t[textLength + 1];
	SendMessage(GetDlgItem(hWnd, id), WM_GETTEXT, textLength + 1, (LPARAM)buffer.c_str());
	Text = buffer;
	//delete[] buffer;
	

}
void ProcessStep(HWND hWnd, vector < vector <pair <BOOL, RECT> > > &steps, int i, int j)
{
	steps[i][j].first = 0;

	if (j == 0)flag1=true;
	if (j == 14)flag2=true;
	if (i == 0)flag3=true;
	if (i == 19)flag4=true;
	

	if (!flag1&&steps[i][j - 1].first == 1)
	{
		steps[i][j - 1].first = 2;//вверх
		UpDateFlags();
		return;
	}

	else
	{

		if (!flag4&&steps[i + 1][j].first == 1)
		{
			steps[i + 1][j].first = 2;//вправо
			UpDateFlags();
			return;
		}
		else
		{

			if (!flag2&&steps[i][j + 1].first == 1)
			{
				steps[i][j + 1].first = 2;//вниз
				UpDateFlags();
				return;
			}
		
		else
		{


			if (!flag3&&steps[i - 1][j].first == 1)
			{
				steps[i - 1][j].first = 2;//влево
				UpDateFlags();
				return;
			}

			else
			{
				gameEnd = true;
				MessageBox(hWnd, (LPCWSTR)(L"Game Over"), (LPCWSTR)L"End", MB_OK);
				
				

				return;
			}
		}
	}
}
	
}

void ProcessTimer(HWND hWnd,vector < vector <pair <BOOL, RECT> > > &steps)
{
	for (int i = 0; i < columns; i++)
	{
		
		for (int j = 0; j < lines; j++)
		{
		
			if (steps[i][j].first == 2)
			{
				ProcessStep(hWnd, steps, i, j);
				return;
			}
		}
	}
}


void OnMouseMove(LPARAM lParam)
{
	POINT p;
	p.x = LOWORD(lParam);
	p.y = HIWORD(lParam);
	for (int i = 0; i < columns; i++)
	{

		for (int j = 0; j < lines; j++)
		{
			if (PtInRect(&steps[i][j].second, p)&& steps[i][j].first==0)
			{
				steps[i][j].first = -1;
				SetCursor(LoadCursor(NULL, IDC_HAND));
				
			}
			if (!PtInRect(&steps[i][j].second, p) && steps[i][j].first == -1)
			{
				steps[i][j].first = 0;
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				
			}
		}
	}
	
}
void OnButtonDown(LPARAM lParam)
{
	POINT p;
	p.x = LOWORD(lParam);
	p.y = HIWORD(lParam);
	for (int i = 0; i < columns; i++)
	{

		for (int j = 0; j < lines; j++)
		{
			if (PtInRect(&steps[i][j].second, p))
			{
				steps[i][j].first = 1;
			}
		}
	}
}
void UpDateFlags()
{
	flag1 = false;
	flag2 = false;
	flag3 = false;
	flag4 = false;
}