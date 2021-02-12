#include "main.hpp"
#include "csgosdk/c_engine.h"
#include <chrono>
#include <thread>
#include "directx/directx.h"
int w = GetSystemMetrics(SM_CXSCREEN);
int h = GetSystemMetrics(SM_CYSCREEN);
const MARGINS Margin = { 0, 0, w, h };

RECT rc;
HWND hWnd;
HWND tWnd;
RECT size_rc;
MSG Message;
LONG topR = 0, leftR = 0, rightR = 0, bottomR = 0;

c_engine g_engine;

void set_window_to_target()
{
	tWnd = FindWindow(0, (LPCWSTR)tWindowName);
	if (tWnd)
	{
		GetWindowRect(tWnd, &size_rc);
		w = size_rc.right - size_rc.left + rightR;
		h = size_rc.bottom - size_rc.top + bottomR;

		size_rc.left -= leftR;
		size_rc.top -= topR;

		DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
		if (dwStyle & WS_BORDER)
		{
			size_rc.top += 23;
			h -= 23;
		}
		MoveWindow(hWnd, size_rc.left, size_rc.top, w, h, true);
	}
	else {
		exit(1);
	}

	while (true)
	{
		tWnd = FindWindow(0, (LPCWSTR)tWindowName);
		if (tWnd)
		{
			GetWindowRect(tWnd, &size_rc);
			w = size_rc.right - size_rc.left + rightR;
			h = size_rc.bottom - size_rc.top + bottomR;

			size_rc.left -= leftR;
			size_rc.top -= topR;

			DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				size_rc.top += 23;
				h -= 23;
			}
			MoveWindow(hWnd, size_rc.left, size_rc.top, w, h, true);
		}
		else
		{
			exit(1);
		}
		Sleep(1);
	}
}

HANDLE hProcess;
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		//RENDER

		if (tWnd == GetForegroundWindow())
		{
			const wchar_t* value = tWindowName;

			HWND newhwnd = FindWindow(NULL, value);
			if (newhwnd != NULL) {
				GetWindowRect(newhwnd, &rc);
			}
			else {
				ExitProcess(0);
			}

			p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
			p_Device->BeginScene();

			DirectX::Render(); // draw

			p_Device->EndScene();
			p_Device->Present(NULL, NULL, NULL, NULL);
		}
		else
		{
			p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
			p_Device->BeginScene();

			p_Device->EndScene();
			p_Device->PresentEx(0, 0, 0, 0, 0);
		}

		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Margin);

		hProcess = process_handle;

		if (!hProcess) {
			exit(1);
		}

		break;

	case WM_DESTROY:
		CloseHandle(hProcess);
		PostQuitMessage(1);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void directx() {
	topR = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 250.0f));
	leftR = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 250.0f));
	bottomR = topR * (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f)) + 1.0f);
	rightR = leftR * (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f)) + 1.0f);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_window_to_target, 0, 0, 0);

	WNDCLASSEX wClass;
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = NULL;
	wClass.lpfnWndProc = WinProc;
	wClass.lpszClassName = L"WINDOW1";
	wClass.lpszMenuName = L"WINDOW1";
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wClass))
		exit(1);

	tWnd = FindWindow(0, tWindowName);
	if (tWnd) {
		GetWindowRect(tWnd, &size_rc);
		if (size_rc.right - size_rc.left < 600) {
			exit(1);
		}

		w = size_rc.right - size_rc.left + rightR;
		h = size_rc.bottom - size_rc.top + bottomR;

		size_rc.left -= leftR;
		size_rc.top -= topR;

		hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_LAYERED, L"WINDOW1", L"WINDOW1", WS_POPUP, 1, 1, w, h, 0, 0, 0, 0);
		SetLayeredWindowAttributes(hWnd, 0, 1.0f, LWA_ALPHA);
		SetLayeredWindowAttributes(hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
		ShowWindow(hWnd, SW_SHOW);
	}

	directx_attach(hWnd);

	for (;; )
	{
		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message);
			TranslateMessage(&Message);
		}
		Sleep(1);
	}
}

int main() {
	// main startup function
	if (!initialize()) {
		_getch();
		return 0;
	}

	// dumping and viewing offsets/netvars/classes
	printf("\n");

	printf(" externally csgo base\n");

	printf("\n");

	printf(" [+]: client: 0x%x\n", client);
	printf(" [+]: engine: 0x%x\n", engine);

	glow team, enemy;

	team.clr = color{ 0, 1, 0 };
	enemy.clr = color{ 1, 0, 0 };

	while (true) {
		// directx attaching
		directx();

		c_base_entity local = g_engine.get_local_player();
		glow_manager = rpm<DWORD>(client + offsets::signatures::dwGlowObjectManager);

		// glow
		for (size_t x = 0; x <= g_engine.get_max_players(); x++) {
			c_base_entity entity = g_engine.get_entity_from_index(x);

			if (entity.address) {
				if (entity.get_team() == local.get_team()) {
					g_engine.glow_entity(entity, team);
				}
				else {
					g_engine.glow_entity(entity, enemy);
				}
			}
		}
		Sleep(1);
	}
	return 0;
}