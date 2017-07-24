#pragma once
#include <Windows.h>

class Dialog
{
	HINSTANCE _hInstance;
	HWND _hDialog;
	PAINTSTRUCT _paintstruct;

public:
	void Create(HINSTANCE hInstance, int resourceID, DLGPROC dialogProcedure);

	void Initialize(HWND hDialog);

	void Show(int nCmdShow);

	BOOL TryProcessMessage(LPMSG message);

	void SetSize(int width, int height);

	void SetCaption(const wchar_t* caption);

	HWND CreateControl(const wchar_t* className, long style, int x, int y, int width, int heigth, int id);

	void Destroy();

	RECT GetClientRect() const;

	HDC BeginPaint();

	void EndPaint();

	void ValidateRect();

	void InvalidateRect();

	operator HWND__*() const
	{
		return _hDialog;
	}
};

