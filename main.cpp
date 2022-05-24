#include <Windows.h>

#define TIMER_ID	1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("KeepAlive");
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szAppName;

	if (!RegisterClass(&wndClass))
	{
		MessageBox(NULL, TEXT("This program requires at least WindowsNT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hWnd = CreateWindow(
		szAppName,
		TEXT("Keep Alive"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, SW_SHOWMINIMIZED);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	static EXECUTION_STATE initialExecutionState;

	INPUT inputs[2] = { 0 };
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_SCROLL;
	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_SCROLL;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	switch (message)
	{
	case WM_CREATE:
		// set the initial window to minimize itself
		initialExecutionState = SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);
		
		// Set up the timer here
		SetTimer(hWnd, TIMER_ID, 10000, NULL);
		return 0;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		DrawText(hDC, TEXT("Keep Alive!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_TIMER:
		// handle the timer by posting two keystrokes in rapid succession
		SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		break;

	case WM_DESTROY:
		// release the timer
		KillTimer(hWnd, TIMER_ID);

		// restore the original execution state
		SetThreadExecutionState(initialExecutionState);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}