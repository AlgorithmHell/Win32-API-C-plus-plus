#pragma once
#include <Windows.h>

class Dialog
{
	HWND _hDialog = nullptr;
	PAINTSTRUCT _paintstruct;

public:
	void Create(HINSTANCE hInstance, int resourceID, DLGPROC dialogProcedure);

	void Initialize(HWND hDialog);

	void Show(int nCmdShow);

	void SetSize(int width, int height);

	void SetCaption(const wchar_t* caption);

	void Destroy();

	RECT GetClientRect() const;

	HDC BeginPaint();

	void EndPaint();

	void ValidateRect();

	void InvalidateRect();

	operator HWND() const;
};

