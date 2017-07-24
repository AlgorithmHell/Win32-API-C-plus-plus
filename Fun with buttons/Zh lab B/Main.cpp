#include <windows.h> 



#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
//VARIABLES TO CHANGE
const int buttonIndent=30, edgeIndent=50, lines=10, columns=10;
const int width = 800, height = 600;
const int buttonWidth = 10, buttonHeight = 10;

//OTHER VARIABLES
int Amount;
int ButtonIndent, EdgeIndent, Lines, Columns;
static int IDD = 1005;

static int  Width=width, Height=height;
static int dx=20, ddx=30;
bool track = false;
HWND MyDialog;
HINSTANCE hInst;
static HDC  hdc, hDC;
PAINTSTRUCT ps;
static vector<HWND> Buttons;
vector <int> IDs;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND CreateControl(const wchar_t*, long, int, int, int, int, int,int,int);
void InitializeButtons();
void InputParametres(int, int, int, int);
void UpdateLayout();





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "SineWave";
	HWND        hwnd;
	MSG         msg;

	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = (LPWSTR)szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow((LPWSTR)szAppName, L"Buttons",
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0,
		0,
	    width,
		height,
		NULL, NULL, hInstance, 0);
	hInst = hInstance;
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (iMsg)
	{
	case WM_CREATE:	
		
		MyDialog = hwnd;
		InputParametres( lines, columns,buttonIndent, edgeIndent );
		InitializeButtons();
		break;

	case WM_LBUTTONDOWN:
	{
		track = true;
		UpdateLayout();
		InvalidateRect(MyDialog, 0, true);
	}
	break;

	

	case WM_SIZE:
		Width = LOWORD(lParam);
		Height = HIWORD(lParam);
		if(track)
	    UpdateLayout();
		return 0;

	

	case WM_DESTROY:
		
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
void InitializeButtons()
{
	int item = 0;
	for (int i=1;i<=Lines;i++)
	{
		for (int j = 1; j <= Columns; j++)
		{
			Buttons[item] = CreateControl(L"BUTTON", 0, dx, dx, buttonWidth, buttonHeight, IDD,i,j);
			dx += ddx;
			IDD++;
			item++;
		}
	}

}

HWND CreateControl(const wchar_t* className,  long style, int x, int y, int width, int heigth, int id,int i,int j)
{
	return CreateWindow(
		className, (LPCWSTR)(to_wstring(i)+L"x"+to_wstring(j)).c_str(),
		WS_CHILD | WS_VISIBLE | style,
		x, y,
		width, heigth,
		MyDialog,
		(HMENU)id,
		(HINSTANCE)hInst,
		nullptr);
}
void InputParametres(int lines, int columns, int buttonIndent, int edgeIndent)
{
	if (lines <= 0 || columns <= 0 || buttonIndent < 0 || edgeIndent < 0)
	{
		auto message=MessageBox(MyDialog, (LPCWSTR)L"Incorrect input,try again",
			(LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
		if (message == IDOK) DestroyWindow(MyDialog);
	}
	Lines = lines;
	Columns = columns;
	ButtonIndent = buttonIndent;
	EdgeIndent = edgeIndent;
	Amount = lines*columns;
	Buttons.resize(Amount);
	IDs.resize(Amount);
}
void UpdateLayout()
{
	RECT rect;
	GetClientRect(MyDialog, &rect);
	int x = rect.left+EdgeIndent;
	int y = rect.top+EdgeIndent;
	int cx = ((rect.right - rect.left)-ButtonIndent*(Columns-1)-2*EdgeIndent) / Columns;
	int cy = ((rect.bottom - rect.top) - ButtonIndent*(Lines - 1) - 2 * EdgeIndent) / Lines;
	if (cx <= 0 || cy <= 0 )
	{
		auto message = MessageBox(MyDialog, (LPCWSTR)L"Incorrect input,try again",
			(LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
		if (message == IDOK) DestroyWindow(MyDialog);
	}
	int item = 0;
	for (int i = 0; i < Lines; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			MoveWindow(Buttons[item], x, y, cx, cy,1);
			item++;
			x += ButtonIndent + cx;
			
		}
		x= rect.left + EdgeIndent;
		y += ButtonIndent + cy;
	}

	
	
}