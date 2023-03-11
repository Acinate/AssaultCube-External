#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define PI 3.14159265

class Paint {
private:
	IDirect3D9Ex* g_pD3D = NULL;
	IDirect3DDevice9Ex* g_pd3dDevice = NULL;
	D3DPRESENT_PARAMETERS g_d3dpp;
	ID3DXFont* font = 0;
	int width, height; 
	HWND TargetHWND;
	HWND OverlayHWND;

	int init(HWND hWND);

public:
	int render();
	void DrawText(char* string, int x, int y, int a, int r, int g, int b);
	void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
	void DrawCircle(int x, int y, int radius, int numSides, DWORD color);
	void ShowGUI();
	void ResetDevice();

	Paint();
	Paint(HWND overlayHWND, HWND targetHWND, int width, int height);
};
