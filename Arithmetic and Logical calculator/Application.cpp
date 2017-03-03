#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "resource.h"
#include <string>


HINSTANCE _hInstance;
HWND _hDialog;
HWND COMBO;
MSG msg;
INT numerator = 0;
INT denominator = 1;
INT result = 0;
BOOL Result = TRUE;
BOOL uVal, bSigned=TRUE;
BOOL ALORLOG = TRUE;
LRESULT COMBO_INDEX;
wchar_t* buffer;
static bool Click = false;

INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL OnCommand(WPARAM);
HWND GetControl(int);
void InitialisationArithmetic();
void InitialisationLogic();
void CalculationArithmetical(INT,INT);
void CalculationLogical(INT, INT );
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{


	_hInstance = hInstance;
	if (!CreateDialogParamW(_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc, 0))
	{
		return FALSE;
	}

	ShowWindow(_hDialog, nCmdShow);
	COMBO = GetControl(IDC_COMBOARITHM);		
	SendMessageW(COMBO, CB_GETCURSEL, 0, 0);

	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		if (!IsDialogMessageW(_hDialog, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return (int)msg.wParam;
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		_hDialog = hDlg;
		
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		return (INT_PTR)OnCommand(wParam);

	case WM_CLOSE:
		DestroyWindow(hDlg);
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}
BOOL OnCommand(WPARAM wParam)
{
	auto controlID = LOWORD(wParam);
	auto notification = HIWORD(wParam);

	switch (controlID)
	{
	case IDC_NUMERATOR:
		switch (notification)
		{
		case EN_CHANGE:
			numerator = GetDlgItemInt(_hDialog, IDC_NUMERATOR, &uVal, bSigned);
			if(Click)
			{
				if (ALORLOG)
				{
					CalculationArithmetical(numerator, denominator);
				}
				else 
				{
					CalculationLogical(numerator, denominator);
				}
			}
			
		}
		break;

	case IDC_DENOMINATOR:
		switch (notification)
		{
		case EN_CHANGE:
			denominator = GetDlgItemInt(_hDialog, IDC_DENOMINATOR, &uVal, bSigned);
			if (Click) 
			{
				if (ALORLOG)
					CalculationArithmetical(numerator, denominator);
				else CalculationLogical(numerator, denominator);
			}
		}
		break;

	case IDC_RESULT:
	{
		if (ALORLOG)
		CalculationArithmetical(numerator, denominator);
	else CalculationLogical(numerator, denominator);
	}
	break;

	case IDC_REFRESH:
	{
		auto send = GetControl(IDC_ResultLabel);
		SendMessageW((HWND)send, WM_SETTEXT, 0, (LPARAM)" ");
		auto fornum = GetControl(IDC_NUMERATOR);
		SendMessageW(fornum, WM_SETTEXT, 0, (LPARAM)" ");
		auto fordenom = GetControl(IDC_DENOMINATOR);
		SendMessageW(fordenom, WM_SETTEXT, 0, (LPARAM)" ");
		COMBO = GetControl(IDC_COMBOARITHM);
		SendMessage(COMBO, CB_SETCURSEL, -1, 0);
		

	}
	break;

	case IDC_COMBOARITHM:
		if (notification == CBN_SELCHANGE)
		{
			COMBO = GetControl(IDC_COMBOARITHM);
			auto Index = SendMessageW(COMBO, CB_GETCURSEL, 0, 0);
			auto textLength = SendMessageW(COMBO, CB_GETLBTEXTLEN, Index, 0);
			auto buf = new wchar_t[textLength + 1]{ 0 };
			SendMessageW(COMBO, CB_GETLBTEXT, Index, (LPARAM)buf);
		}
		if (Click)
		{
			if (ALORLOG)
				CalculationArithmetical(numerator, denominator);
			else CalculationLogical(numerator, denominator);
		}
		break;
	case IDC_AUTO:
	{
		switch (notification)
		{
			
		case BN_CLICKED:
		{
			auto invisible = GetControl(IDC_RESULT);
			if (Click) {
				ShowWindow(invisible, SW_SHOW);
				Click = false;


			}
			else
			{
				Click = true;
				ShowWindow(invisible, SW_HIDE);
				
			}
			break;
		}
		}
	}
		break;
	case IDC_ARITHMETICAL:
	{
		ALORLOG = TRUE;
		SendDlgItemMessage(_hDialog, IDC_COMBOARITHM, CB_RESETCONTENT, 0, 0);
		InitialisationArithmetic();
	}
	break;
	case IDC_LOGICAL:
	{
		ALORLOG = FALSE;
		SendDlgItemMessage(_hDialog, IDC_COMBOARITHM, CB_RESETCONTENT, 0, 0);
		InitialisationLogic();
	}
	break;
		return FALSE;
	}
}


HWND GetControl(int id)
{
	return GetDlgItem(_hDialog, id);
}
void InitialisationArithmetic()
{
	COMBO = GetControl(IDC_COMBOARITHM);
	SendMessage(COMBO, CB_ADDSTRING, 0, (LPARAM)"-");	
	SendMessage(COMBO, CB_ADDSTRING, 0, (LPARAM)"*");
	SendMessage(COMBO, CB_ADDSTRING, 0, (LPARAM)"/");
	SendMessage(COMBO, CB_ADDSTRING, 0, (LPARAM)"+");
	
	
}
void InitialisationLogic()
{
	COMBO = GetControl(IDC_COMBOARITHM);	
	SendMessage(COMBO, CB_ADDSTRING, 0, (LPARAM)L"!=");
	SendMessage(COMBO, CB_ADDSTRING, 0, (LPARAM)L"<");
	SendMessage(COMBO, CB_ADDSTRING, 0, (LPARAM)L"=");
	SendMessage(COMBO, CB_ADDSTRING, 0, (LPARAM)L">");


}
void CalculationArithmetical(INT numerator, INT denominator)

	{	COMBO = GetControl(IDC_COMBOARITHM);
	int selectedIndex = SendMessageW(COMBO, CB_GETCURSEL, 0, 0);
	if (selectedIndex == 2)
	{
		if (denominator != 0) {
			result = numerator / denominator;

			SetDlgItemInt(_hDialog, IDC_ResultLabel, result, bSigned);
			
		}
		else {
			MessageBox(_hDialog, L"Division by zero!", L"Error", MB_OK|MB_ICONERROR);
			
		}
	}
	else
	{
		if (selectedIndex == 1) {
			result = numerator *denominator;

			SetDlgItemInt(_hDialog, IDC_ResultLabel, result, bSigned);
			
		}
		if (selectedIndex == 3) {
			result = numerator + denominator;

			SetDlgItemInt(_hDialog, IDC_ResultLabel, result, bSigned);
			
		}
		if (selectedIndex == 0) {
			result = numerator - denominator;

			SetDlgItemInt(_hDialog, IDC_ResultLabel, result, bSigned);
			
		}


	}
	
}
void CalculationLogical(INT numerator, INT denominator)

{
	COMBO = GetControl(IDC_COMBOARITHM);
	int selectedIndex = SendMessageW(COMBO, CB_GETCURSEL, 0, 0);
	switch (selectedIndex)
	{
	case 0:
	{
		if (numerator != denominator)
			SendMessage(GetControl(IDC_ResultLabel), WM_SETTEXT, 0, (LPARAM)L"TRUE");
		else SendMessage(GetControl(IDC_ResultLabel), WM_SETTEXT, 0, (LPARAM)L"FALSE");
	}
	break;
	case 1:
	{
		if (numerator < denominator)
			SendMessage(GetControl(IDC_ResultLabel), WM_SETTEXT, 0, (LPARAM)L"TRUE");
		else SendMessage(GetControl(IDC_ResultLabel), WM_SETTEXT, 0, (LPARAM)L"FALSE");
	}
	break;
	case 2:
	{
		if (numerator == denominator)
			SendMessage(GetControl(IDC_ResultLabel), WM_SETTEXT, 0, (LPARAM)L"TRUE");
		else SendMessage(GetControl(IDC_ResultLabel), WM_SETTEXT, 0, (LPARAM)L"FALSE");
	}
	break;
	case 3:
	{
		if (numerator > denominator)
			SendMessage(GetControl(IDC_ResultLabel), WM_SETTEXT, 0, (LPARAM)L"TRUE");
		else SendMessage(GetControl(IDC_ResultLabel), WM_SETTEXT, 0, (LPARAM)L"FALSE");
	}
	break;
	}
	

	

}