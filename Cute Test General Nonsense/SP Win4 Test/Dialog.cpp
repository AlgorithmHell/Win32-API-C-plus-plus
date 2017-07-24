#include "Dialog.h"
#include <exception>

void Dialog::Create(HINSTANCE hInstance, int resourceID, DLGPROC dialogProcedure)
{
	_hInstance = hInstance;
	if (!CreateDialog(hInstance, MAKEINTRESOURCE(resourceID), nullptr, dialogProcedure))
	{
		throw std::exception("Can't create dialog.");
	}
}

void Dialog::Initialize(HWND hDialog)
{
	_hDialog = hDialog;
}

void Dialog::Show(int nCmdShow)
{
	::ShowWindow(_hDialog, nCmdShow);
}

BOOL Dialog::TryProcessMessage(LPMSG message)
{

	return ::IsDialogMessage(_hDialog, message);
}

void Dialog::SetSize(int width, int height)
{
	::SetWindowPos(_hDialog, nullptr, NULL, NULL, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Dialog::SetCaption(const wchar_t* caption)
{
	::SetWindowText(_hDialog, caption);
}

HWND Dialog::CreateControl(const wchar_t* className, long style, int x, int y, int width, int heigth, int id)
{
	return CreateWindow(
		className, L"",
		WS_CHILD | WS_VISIBLE | style,
		x, y,
		width, heigth,
		_hDialog,
		(HMENU)id,
		_hInstance,
		nullptr);
}

void Dialog::Destroy()
{
	::DestroyWindow(_hDialog);
}

RECT Dialog::GetClientRect() const
{
	RECT rect;
	::GetClientRect(_hDialog, &rect);
	return rect;
}

HDC Dialog::BeginPaint()
{
	return ::BeginPaint(_hDialog, &_paintstruct);
}

void Dialog::EndPaint()
{
	::EndPaint(_hDialog, &_paintstruct);
}

void Dialog::ValidateRect()
{
	::ValidateRect(_hDialog, nullptr);
}

void Dialog::InvalidateRect()
{
	::InvalidateRect(_hDialog, nullptr, TRUE);
}


