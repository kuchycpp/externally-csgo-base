
#ifndef D3D9
#define D3D9
#include <d3dx9.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

IDirect3DDevice9Ex* p_Device = 0;
D3DXVECTOR3 position;

DWM_BLURBEHIND bb = { DWM_BB_ENABLE | DWM_BB_BLURREGION, true, CreateRectRgn(0, 0, -1, -1), true };
D3DCOLOR colorprim = D3DCOLOR_ARGB(255, 203, 111, 111); //PRIMARY CHEAT COLOR 
D3DCOLOR colorsec = D3DCOLOR_ARGB(200, 5, 0, 0); //SECONDARY CHEAT COLOR 

extern IDirect3DDevice9Ex* p_Device;
#define PI 3.14159265 
extern LONG leftR, topR, bottomR;

#include <sstream>
using namespace std;
using std::stringstream;

namespace draw {

	void draw_string(char* String, int x, int y, int a, int r, int g, int b, ID3DXFont* font) {
		x += leftR;
		y += topR;
		RECT FontPos;
		FontPos.left = x;
		FontPos.top = y;
		font->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
	}
}

extern int w, h;
IDirect3D9Ex* p_Object = 0;
D3DPRESENT_PARAMETERS p_Params;

int directx_attach(HWND hWnd) {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(1);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = w;
	p_Params.BackBufferHeight = h;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&p_Params,
		0,
		&p_Device)))
		exit(1);
	return 0;
}

extern HWND hWnd;

namespace DirectX
{
	void Render()
	{
		SetLayeredWindowAttributes(hWnd, NULL, NULL, NULL);
		DwmEnableBlurBehindWindow(hWnd, &bb);

		ID3DXFont* pFont;
		D3DXCreateFont(p_Device, 12, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Verdana", &pFont);

		draw::draw_string((char*)("externally-csgo-base"), 10, 10, 255, 255, 255, 255, pFont);
	}
}

#endif
