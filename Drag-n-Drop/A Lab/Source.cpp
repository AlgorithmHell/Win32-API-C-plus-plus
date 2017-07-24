#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "resource.h"
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

//VARIABLES
const int width = 1000, height = 500;
const int FirstPos = 215, SecondPos = 615, TextWidth = 20;
HINSTANCE hInst;

RECT Rect;
RECT FirstBox;
RECT SecondBox;
HBRUSH BlueBrush, YellowBrush, StrawBerryBrush, OrangeBrush;
const int AmountOfObjects =4 ;

RECT MyObjects[AmountOfObjects];
bool ObjectIsDragged[AmountOfObjects];
int FirstBoxCount = 4;
int SecondBoxCount = 0;

int PrevXcoordinate = 0;
int PrevYcoordinate = 0;


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
void DrawRectangle(const HDC& deviceContext, const RECT& rect)
{
	Rectangle(deviceContext, rect.left, rect.top, rect.right, rect.bottom);
}

void SetRectFields(RECT* rect, int x, int y, int width, int height)
{
	SetRect(rect, x, y, x + width, y + height);
}

void HoveredObjectDragState(HWND hDlg, bool State)
{
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(hDlg, &mousePosition);
	for (int i = AmountOfObjects - 1; i >= 0; i--) {
		auto isHoverRect = PtInRect(&MyObjects[i], mousePosition);
		if (isHoverRect) {
			
			ObjectIsDragged[i] = State;
			if (State ) break;

		}

	}
}

bool SomethingIsDragged()
{
	for (int i = 0; i < AmountOfObjects; i++)
		if (ObjectIsDragged[i]) {
			SetCursor(LoadCursor(NULL, IDC_HAND));
			return true;

		}
	SetCursor(LoadCursor(NULL, IDC_NO));
	return false;
}

bool RectsAreEqual(const RECT& r1, const RECT& r2)
{
	return (r1.bottom == r2.bottom &&
		r1.left == r2.left &&
		r1.right == r2.right &&
		r1.top == r2.top);
}

void UpdateCounters(HWND MyDialog)
{
	FirstBoxCount = 0;
	SecondBoxCount = 0;
	for (int i = 0; i < AmountOfObjects; i++) {
		IntersectRect(&Rect, &FirstBox, &MyObjects[i]);
		if (RectsAreEqual(Rect, MyObjects[i]))
			FirstBoxCount++;
		IntersectRect(&Rect, &SecondBox, &MyObjects[i]);
		if (RectsAreEqual(Rect, MyObjects[i]))
			SecondBoxCount++;

	}
}

void CalculateRects(HWND MyDialog)
{
	auto top = 50;
	auto left = 50;
	auto rectSize = 350;
	auto objectSize = 100;
	auto offset = 50;

	SetRectFields(&FirstBox, left, top, rectSize, rectSize);
	SetRectFields(&SecondBox, left + rectSize + offset, top, rectSize, rectSize);

	SetRectFields(&MyObjects[0], left + offset, top + offset, objectSize, objectSize);
	SetRectFields(&MyObjects[1], left + objectSize + 2 * offset, top + offset, objectSize, objectSize);
	SetRectFields(&MyObjects[2], left + offset, top + objectSize + 2 * offset, objectSize, objectSize);
	SetRectFields(&MyObjects[3], left + objectSize + 2 * offset, top + objectSize + 2 * offset, objectSize, objectSize);
	for (int i = 0; i < AmountOfObjects; i++)
		ObjectIsDragged[i] = false;
}

void DrawBoxes(HDC& hdc)
{
	SelectObject(hdc, OrangeBrush);
	DrawRectangle(hdc, FirstBox);
	DrawRectangle(hdc, SecondBox);
}

void DrawObjects(HDC& hdc, HWND MyDialog)
{
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	ScreenToClient(MyDialog, &mousePosition);

	for (int i = 0; i < AmountOfObjects; i++) {
		auto isHoverRect = PtInRect(&MyObjects[i], mousePosition);
		SelectObject(hdc, BlueBrush);
		if (isHoverRect)
			SelectObject(hdc, YellowBrush);
		if (ObjectIsDragged[i])
			SelectObject(hdc, StrawBerryBrush);
		DrawRectangle(hdc, MyObjects[i]);

	}
}

void DrawCounters(HDC& hdc)
{
	SetBkMode(hdc, TRANSPARENT);
	
	TextOut(hdc, FirstPos, TextWidth, std::to_wstring(FirstBoxCount).c_str(), std::to_string(FirstBoxCount).size());
	TextOut(hdc, SecondPos, TextWidth, std::to_wstring(SecondBoxCount).c_str(), std::to_string(SecondBoxCount).size());
}

void OnPaint(HWND MyDialog)
{
	PAINTSTRUCT ps;
	GetClientRect(MyDialog, &Rect);
	int width = Rect.right - Rect.left;
	int height = Rect.bottom - Rect.top;
	auto MyDC = BeginPaint(MyDialog, &ps);
	
	auto hdc = CreateCompatibleDC(MyDC);
	auto CompBitmap = CreateCompatibleBitmap(MyDC, width, height);
	auto OldBitmap = (HBITMAP)SelectObject(hdc, CompBitmap);
	PatBlt(hdc, 0, 0, width, height, WHITENESS);
	

	DrawBoxes(hdc);
	DrawObjects(hdc, MyDialog);
	DrawCounters(hdc);

	
	BitBlt(MyDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	DeleteObject(CompBitmap);
	DeleteDC(hdc);
	
	EndPaint(MyDialog, &ps);
}

void DoNotAllowCursorToLeaveTheWindow(HWND MyDialog)
{
	
	GetClientRect(MyDialog, &Rect);

	POINT positionLeftTop = { Rect.left, Rect.top };
	ClientToScreen(MyDialog, &positionLeftTop);
	POINT positionRightBottom = { Rect.right, Rect.bottom };
	ClientToScreen(MyDialog, &positionRightBottom);

	RECT rectCapture;

	//SetRect(&rectCapture, Rect.left, Rect.top, Rect.right, Rect.bottom);
	SetRect(&rectCapture, positionLeftTop.x, positionLeftTop.y, positionRightBottom.x, positionRightBottom.y);
	ClipCursor(&rectCapture);
}

void MoveObject(RECT& rect, int dx, int dy)
{
	rect.left += dx;
	rect.right += dx;
	rect.top += dy;
	rect.bottom += dy;
}

void OnMouseMove(HWND MyDialog, int x, int y)
{
	int dx = x - PrevXcoordinate;
	int dy = y- PrevYcoordinate;

	for (int i = 0; i < AmountOfObjects; i++)
		if (ObjectIsDragged[i])
			MoveObject(MyObjects[i], dx, dy);

	PrevXcoordinate = x;
	PrevYcoordinate = y;

	if (SomethingIsDragged())
		DoNotAllowCursorToLeaveTheWindow(MyDialog);
	else
		ClipCursor(nullptr);
	UpdateCounters(MyDialog);
	InvalidateRect(MyDialog, nullptr, false);
}


LRESULT CALLBACK DialogProcedure(HWND MyDialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		CalculateRects(MyDialog);
		CreateBrushes();
		return TRUE;

	case WM_CLOSE:
		
		DeleteBrushes();
		EndDialog(MyDialog, 0);
		return TRUE;
	case WM_PAINT:
		OnPaint(MyDialog);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(MyDialog, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN: {
		PrevXcoordinate = LOWORD(lParam);
		PrevYcoordinate = HIWORD(lParam);
		HoveredObjectDragState(MyDialog, true);
		InvalidateRect(MyDialog, nullptr, false);
		break;
	}
	case WM_LBUTTONUP: {
		HoveredObjectDragState(MyDialog, false);
		InvalidateRect(MyDialog, nullptr, false);
		break;
	}

	}
	return FALSE;
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	hInst = hInstance;
	
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_MyDialog), NULL, (DLGPROC)DialogProcedure);
}
