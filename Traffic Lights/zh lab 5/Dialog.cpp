#include "Dialog.h"
#include <exception>

void Dialog::Create(HINSTANCE hInstance, int resourceID, DLGPROC dialogProcedure)
{
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

void Dialog::SetSize(int width, int height)
{
	::SetWindowPos(_hDialog, nullptr, NULL, NULL, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Dialog::SetCaption(const wchar_t* caption)
{
	::SetWindowText(_hDialog, caption);
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

Dialog::operator HWND () const
{
	return _hDialog;
}
