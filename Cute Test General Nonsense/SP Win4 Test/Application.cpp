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
#include <codecvt>
#include<fstream>
#include<string>
#include<iomanip>
#include <iterator>
#include <sstream>
using namespace std;




HINSTANCE hInst;
Dialog dialog;
MSG msg;
HDC hdc, hDC;
HBRUSH hNew, hOld, Brush;
HMENU hMenu;
HINSTANCE hInstance;
vector< pair<wstring,vector<wstring> > > Questions;
static int Next = 0;
const int AmountOfQuestions = 10;
static int SelAnswer;
static int ticks = 10;
int Answers[10] = { 3,2,1,2,4,1,4,2,3,4 };
vector <bool> RightAnswers;

static void OnInitialize();
void SetDialogProperties();

void OnCommand(WPARAM wParam, HINSTANCE hInstance,HWND);
void OnCommandTest(WPARAM wParam, HWND hWnd);
void ResetQuestion(int,HWND);
HWND GetControl(int id);
void NextProcess(HWND hWnd);

INT_PTR CALLBACK GameProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
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

static INT_PTR CALLBACK DialogProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
		hMenu = GetMenu(hDialog);
		hMenu = GetSubMenu(hMenu, 0);
		break;
	case WM_INITDIALOG:
		dialog.Initialize(hDialog);
		ShowWindow(GetDlgItem(hDialog, IDC_STATIC1), SW_HIDE);
		ShowWindow(GetDlgItem(hDialog, IDC_Total), SW_HIDE);
		OnInitialize();
		return (INT_PTR)TRUE;
	
	case WM_CLOSE:
		
		dialog.Destroy();
		return TRUE;

	case WM_COMMAND:
		OnCommand(wParam, hInstance,hDialog);
		InvalidateRect(hDialog, 0, false);
		break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK GameProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_INITDIALOG:
		RightAnswers.resize(10);
		SendMessage(GetDlgItem(hWnd, IDC_Question), WM_SETTEXT, 0, (LPARAM)Questions[Next].first.c_str());
		SendMessage(GetDlgItem(hWnd, IDC_Answer1), WM_SETTEXT, 0, (LPARAM)Questions[Next].second[0].c_str());
		SendMessage(GetDlgItem(hWnd, IDC_Answer2), WM_SETTEXT, 0, (LPARAM)Questions[Next].second[1].c_str());
		SendMessage(GetDlgItem(hWnd, IDC_Answer3), WM_SETTEXT, 0, (LPARAM)Questions[Next].second[2].c_str());
		SendMessage(GetDlgItem(hWnd, IDC_Answer4), WM_SETTEXT, 0, (LPARAM)Questions[Next].second[3].c_str());
		
		SetTimer(hWnd, 1, 1000, NULL);
		break;
	case WM_TIMER:		
		
		ticks--;
		SendMessage(GetDlgItem(hWnd, IDC_Time), WM_SETTEXT, 0, (LPARAM)to_wstring(ticks).c_str());
		if (ticks == 0)
		{
			ticks = 10;
			NextProcess(hWnd);
		}
		
		break;

		break;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		int c = x + y;
		break;
	}
	case WM_COMMAND:
	{

		OnCommandTest(wParam, hWnd);
	}
	break;




	case WM_CLOSE:
	{
		Next = 0;
		EndDialog(hWnd, 0);
	}
	return TRUE;



	}
	return (INT_PTR)FALSE;
}
static void OnInitialize()
{
	ifstream of("input.txt");
	string line;
	
	Questions.resize(10);
	
	for (auto i=0;i<AmountOfQuestions;i++)
	{
		Questions[i].second.resize(4);
	}

	for (int i=0;i<AmountOfQuestions;i++)
	{
		getline(of, line);
		wstring buf(line.length(), L' ');
		
		copy(line.begin(), line.end(), buf.begin());
		Questions[i].first = buf;
		for (int j = 0; j < 4; j++)
		{ 
			getline(of, line);
			wstring otherBuf(line.length(), L' ');
			copy(line.begin(), line.end(), otherBuf.begin());
			Questions[i].second[j] = otherBuf;
		}

	}


}
void SetDialogProperties()
{

	dialog.SetCaption(L"General Nonsense");
}

void OnCommand(WPARAM wParam, HINSTANCE hInstance,HWND hDialog)
{
	switch (LOWORD(wParam))
	{
	case ID_NEW_STARTTEST:
	{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_TestDialog), dialog, GameProc);
	int Total = 0;
	for (int i = 0; i < 10; i++)
	{
		if (RightAnswers[i] == true)
			Total++;
	}
	SendMessage(GetDlgItem(hDialog, IDC_Total), WM_SETTEXT, 0, (LPARAM)to_wstring(Total).c_str());
	ShowWindow(GetDlgItem(hDialog, IDC_STATIC1), SW_SHOW);
	ShowWindow(GetDlgItem(hDialog, IDC_Total), SW_SHOW);
	break;
	}

	}


}
void OnCommandTest(WPARAM wParam, HWND hWnd)

{
	switch (LOWORD(wParam))
	{
	
	case IDC_Answer1:
		SelAnswer = 1;
		NextProcess(hWnd);
		break;
	case IDC_Answer2:
		SelAnswer = 2;
		NextProcess(hWnd);
		break;
	case IDC_Answer3:
		SelAnswer = 3;
		NextProcess(hWnd);
		break;
	case IDC_Answer4:
		SelAnswer = 4;
		NextProcess(hWnd);
		break;
	case IDC_NEXT:
	{
		NextProcess(hWnd);		

	}
		break;
	}
}
HWND GetControl(int id)
{
	return GetDlgItem(dialog, id);
}
void ResetQuestion(int Next,HWND hWnd)
{
	SendMessage(GetDlgItem(hWnd, IDC_Question), WM_SETTEXT, 0, (LPARAM)Questions[Next].first.c_str());
	SendMessage(GetDlgItem(hWnd, IDC_Answer1), WM_SETTEXT, 0, (LPARAM)Questions[Next].second[0].c_str());
	SendMessage(GetDlgItem(hWnd, IDC_Answer2), WM_SETTEXT, 0, (LPARAM)Questions[Next].second[1].c_str());
	SendMessage(GetDlgItem(hWnd, IDC_Answer3), WM_SETTEXT, 0, (LPARAM)Questions[Next].second[2].c_str());
	SendMessage(GetDlgItem(hWnd, IDC_Answer4), WM_SETTEXT, 0, (LPARAM)Questions[Next].second[3].c_str());
	SendMessage(GetDlgItem(hWnd, IDC_Answer1), BM_SETCHECK, BST_UNCHECKED, 0);
	SendMessage(GetDlgItem(hWnd, IDC_Answer2), BM_SETCHECK, BST_UNCHECKED, 0);
	SendMessage(GetDlgItem(hWnd, IDC_Answer3), BM_SETCHECK, BST_UNCHECKED, 0);
	SendMessage(GetDlgItem(hWnd, IDC_Answer4), BM_SETCHECK, BST_UNCHECKED, 0);

	
	SendMessage(GetDlgItem(hWnd, IDC_Process), WM_SETTEXT, 0, (LPARAM) (to_wstring(Next+1)+L"/10").c_str());
}
void NextProcess(HWND hWnd)
{
	if (SelAnswer == Answers[Next])
	{
		RightAnswers[Next] = true;
	}
	else
	{
		RightAnswers[Next] = false;
	}

	if (Next == 9)
	{
		EndDialog(hWnd, 0);
	}
	else {
		Next++;
		ticks = 10;
		SendMessage(GetDlgItem(hWnd, IDC_Time), WM_SETTEXT, 0, (LPARAM)to_wstring(ticks).c_str());
		ResetQuestion(Next, hWnd);
	}
}
/*
Таймер Рисование События мыши Диалоговые окна
в окне два поля с текстом и кнопка окей
что-то делается и вызывается перерисовка
какие-то объекты если на них кликнуть что-то происходит
*/