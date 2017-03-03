#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <CommCtrl.h>
#include "resource.h"
const int MIN=0;
const int MAX = 10;
static int Value = 0;
HINSTANCE _hInstance=nullptr;
HWND MyDialog=nullptr;
HWND MySlider=nullptr;
HWND Image_1=nullptr;
HWND Image_2 = nullptr;
HWND Image_3 = nullptr;
HWND MyLabel=nullptr;

void SetValue(int);
void OnInitialize();
void InitializeDialog();
void InitializeSlider();
void InitializeLabel();
void InitializeImage_1();
void InitializeImage_2();
void InitializeImage_3();
void SendImage_1();
void SendImage_2();
void SendImage_3();

HWND CreateControl(const wchar_t*, long, int, int, int, int, int);
BOOL OnHorizontalScroll(WPARAM, LPARAM);
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

namespace Resources
{
	const int SliderID = 1002;
	const int Image1_ID = 1003;
	const int Image2_ID = 1004;
	const int Image3_ID = 1005;
	const int Label_ID = 1006;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_hInstance = hInstance;
	if (!CreateDialogParamW(_hInstance, MAKEINTRESOURCE(IDD_DIALOG), nullptr, DialogProc, 0))
	{
		return FALSE;
	}

	ShowWindow(MyDialog, nCmdShow);

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		if (!IsDialogMessageW(MyDialog, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return (int)msg.wParam;
}
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		MyDialog = hDlg;
		OnInitialize();
		return (INT_PTR)TRUE;

	case WM_HSCROLL:
	 return (INT_PTR)OnHorizontalScroll(wParam, lParam);


	case WM_CLOSE:
		DestroyWindow(hDlg);
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}

void SetValue(int value)
{
	Value = value;
	SendMessage(MySlider, TBM_SETPOS, TRUE, Value);
	auto GetLabel = GetDlgItem(MyDialog, Resources::Label_ID);
	SetDlgItemInt(MyDialog,Resources::Label_ID,(UINT)Value,FALSE);
	
}

void OnInitialize()
{
	InitializeDialog();	
	InitializeSlider();
	InitializeLabel();
	InitializeImage_1();
	SendImage_1();
	InitializeImage_2();
	InitializeImage_3();
	SetValue(MIN);
}

void InitializeDialog()
{
	SetWindowPos(MyDialog, nullptr, 800, 1200, 300, 250, SWP_NOMOVE | SWP_NOZORDER);
	SetWindowText(MyDialog, L"Testing slider and images");
}

void InitializeSlider()
{
	MySlider = CreateControl(TRACKBAR_CLASS, WS_TABSTOP | TBS_AUTOTICKS | TBS_HORZ, 20, 60, 150, 32, Resources::SliderID);
	SendMessage(MySlider, TBM_SETRANGE, TRUE, MAKELONG(MIN, MAX));
}

void InitializeLabel()
{
	MyLabel = CreateControl( L"STATIC",0,125, 120, 100, 20,Resources::Label_ID);
	
}

void InitializeImage_1()
{
	Image_1 = CreateControl(L"STATIC", SS_BITMAP, 200, 20, NULL, NULL, Resources::Image1_ID);
	
}
void InitializeImage_2()
{
	Image_2= CreateControl(L"STATIC", SS_BITMAP, 200, 20, NULL, NULL, Resources::Image2_ID);
	auto bmp = (HBITMAP)LoadImage(nullptr, L"Neutral.bmp", IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);
	
}
void InitializeImage_3()
{
	Image_3 = CreateControl(L"STATIC", SS_BITMAP, 200, 20, NULL, NULL, Resources::Image3_ID);
	auto bmp = (HBITMAP)LoadImage(nullptr, L"Happy.bmp", IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);
	
}
void SendImage_1()
{
	auto bmp = (HBITMAP)LoadImage(nullptr, L"Sad.bmp", IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);
	SendMessage(Image_1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
}
void SendImage_2()
{
	auto bmp = (HBITMAP)LoadImage(nullptr, L"Neutral.bmp", IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);
	SendMessage(Image_2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
}
void SendImage_3()
{
	auto bmp = (HBITMAP)LoadImage(nullptr, L"Happy.bmp", IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);
	SendMessage(Image_3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
}

HWND CreateControl(const wchar_t* className, long style, int x, int y, int width, int heigth, int id)
{
	return CreateWindow(
		className, L"",
		WS_CHILD | WS_VISIBLE | style,
		x, y,
		width, heigth,
		MyDialog,
		(HMENU)id,
		_hInstance,
		nullptr);
}
BOOL OnHorizontalScroll(WPARAM wParam, LPARAM lParam)
{
	auto hControl = (HWND)lParam;
	if (hControl != MySlider) return FALSE;
	auto value = SendMessage(MySlider, TBM_GETPOS, NULL, NULL);
	SetValue(value);

	if (value >= 0 && value < 4) SendImage_1();
	if (value >= 4 && value < 7) SendImage_2();
	if (value >= 7 && value < 10) SendImage_3();
	return TRUE;
}

