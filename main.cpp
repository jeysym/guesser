#include <Windows.h>
#include "resource.h"
#include <string>

static HINSTANCE g_instance;

static int g_lowerBound = 1;
static int g_currentGuess = 500;
static int g_upperBound = 999;
static int g_numTries = 1;

BOOL CALLBACK DialogProc(HWND dialogWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND guessNumberWnd = GetDlgItem(dialogWindow, IDC_GUESS_NUMBER);
	HWND triesNumberWnd = GetDlgItem(dialogWindow, IDC_TRIES);

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_GREATER_BTN:
		{
			PlaySound(MAKEINTRESOURCE(IDW_BELL), g_instance, SND_RESOURCE | SND_SYNC);
			PlaySound(MAKEINTRESOURCE(IDW_MUSIC), g_instance, SND_RESOURCE | SND_LOOP | SND_ASYNC);

			g_lowerBound = g_currentGuess + 1;
			g_currentGuess = (g_lowerBound + g_upperBound) / 2;
			std::wstring currentGuessStr = std::to_wstring(g_currentGuess);
			SetWindowText(guessNumberWnd, currentGuessStr.c_str());

			g_numTries += 1;
			std::wstring numTriesStr = std::to_wstring(g_numTries);
			SetWindowText(triesNumberWnd, numTriesStr.c_str());

			return TRUE;
		}
		case IDC_LOWER_BTN:
		{
			PlaySound(MAKEINTRESOURCE(IDW_BELL), g_instance, SND_RESOURCE | SND_SYNC);
			PlaySound(MAKEINTRESOURCE(IDW_MUSIC), g_instance, SND_RESOURCE | SND_LOOP | SND_ASYNC);

			g_upperBound = g_currentGuess - 1;
			g_currentGuess = (g_lowerBound + g_upperBound) / 2;
			std::wstring currentGuessStr = std::to_wstring(g_currentGuess);
			SetWindowText(guessNumberWnd, currentGuessStr.c_str());

			g_numTries += 1;
			std::wstring numTriesStr = std::to_wstring(g_numTries);
			SetWindowText(triesNumberWnd, numTriesStr.c_str());

			return TRUE;
		}
		case IDC_MATCH_BTN:
			std::wstring message = std::wstring(L"Yay, I guessed it right in ") + std::to_wstring(g_numTries) + std::wstring(L" tries!");
			MessageBox(dialogWindow, message.c_str(), L"I won!", MB_OK);
			PostQuitMessage(0);
			return TRUE;
		}
	}

	return FALSE;
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(window, message, wParam, lParam);
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nShowCmd)
{
	g_instance = hInstance;

	WNDCLASSEX windowClassDesc;
	windowClassDesc.cbSize = sizeof(WNDCLASSEX);
	windowClassDesc.style = CS_OWNDC;
	windowClassDesc.lpfnWndProc = WindowProc;
	windowClassDesc.cbClsExtra = 0;
	windowClassDesc.cbWndExtra = 0;
	windowClassDesc.hInstance = hInstance;
	windowClassDesc.hIcon = 0;
	windowClassDesc.hCursor = 0;
	windowClassDesc.hbrBackground = 0;
	windowClassDesc.lpszMenuName = 0;
	windowClassDesc.lpszClassName = L"GUESSER_WINDOW_CLASS";
	windowClassDesc.hIconSm = 0;

	ATOM windowClass = RegisterClassEx(&windowClassDesc);

	HWND window = CreateWindow(L"GUESSER_WINDOW_CLASS", L"My Window", WS_OVERLAPPEDWINDOW, 0, 0, 200, 250, 0, 0, hInstance, 0);
	ShowWindow(window, SW_SHOW);
	UpdateWindow(window);

	HWND dialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), window, (DLGPROC)DialogProc);
	ShowWindow(dialog, SW_SHOW);
	UpdateWindow(dialog);

	HFONT font = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Comic Sans MS");
	SendDlgItemMessage(dialog, IDC_GUESS_NUMBER, WM_SETFONT, (WPARAM)font, TRUE);
	HFONT font2 = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Comic Sans MS");
	SendDlgItemMessage(dialog, IDC_QUESTION, WM_SETFONT, (WPARAM)font2, TRUE);
	
	MessageBox(dialog, L"Think of a number in range [1-999]", L"Think of a number", MB_OK);

	MSG message;
	BOOL result;
	while ((result = GetMessage(&message, NULL, 0, 0)) != 0)
	{
		if (result == -1)
		{
			break;
		}
		else if (!IsWindow(dialog) || !IsDialogMessage(dialog, &message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return message.wParam;
}