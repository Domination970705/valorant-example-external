//#pragma once
//#include <Windows.h>
//#include <d3d9.h>
//#include "DirectX9/Include/d3dx9.h"
//#include <dwmapi.h>
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_win32.h"
//#include "imgui/imgui_impl_dx9.h"
//#include "sdk.h"
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "Dwmapi.lib")
//#include "font.h"
//#include "iconcpp.h"
//#include "icons.h"
//IDirect3D9Ex* p_Object = NULL;
//IDirect3DDevice9Ex* p_Device = NULL;
//D3DPRESENT_PARAMETERS p_Params = { NULL };
//
//HWND MyWnd = NULL;
//HWND GameWnd = NULL;
//MSG Message = { NULL };
//
//RECT GameRect = { NULL };
//D3DPRESENT_PARAMETERS d3dpp;
//
//DWORD ScreenCenterX;
//DWORD ScreenCenterY;
//
//static ULONG Width = GetSystemMetrics(SM_CXSCREEN);
//static ULONG Height = GetSystemMetrics(SM_CYSCREEN);
//auto init_wndparams(HWND hWnd) -> HRESULT
//{
//	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
//		exit(3);
//
//	ZeroMemory(&p_Params, sizeof(p_Params));
//	p_Params.Windowed = TRUE;
//	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
//	p_Params.hDeviceWindow = hWnd;
//	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
//	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
//	p_Params.BackBufferWidth = Width;
//	p_Params.BackBufferHeight = Height;
//	p_Params.EnableAutoDepthStencil = TRUE;
//	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
//	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
//	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device))) {
//		p_Object->Release();
//		exit(4);
//	}
//	ImGui::CreateContext();
//
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	io.IniFilename = nullptr;
//
//	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
//	ImFont* default_segu = nullptr;
//	ImFont* ico = nullptr;
//	ImFont* ico_combo = nullptr;
//	ImFont* ico_button = nullptr;
//	ImFont* ico_grande = nullptr;
//	ImFont* segu = nullptr;
//	ImFont* bold_segu = nullptr;
//	ImFontConfig icons_config;
//
//	ImFontConfig CustomFont;
//	ImFontConfig font_config;
//	static const ImWchar ranges[] =
//	{
//		0x0020, 0x00FF, // Basic Latin Supplement
//		0x0400, 0x052F, // Supplement
//		0x2DE0, 0x2DFF, // Extended-A
//		0xA640, 0xA69F, // Extended-B
//		0xE000, 0xE226, // Icons
//		0,
//	};
//	CustomFont.FontDataOwnedByAtlas = false;
//
//	io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 17.f, &CustomFont);
//
//	io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	default_segu = io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	segu = io.Fonts->AddFontFromMemoryTTF(&seguoe, sizeof seguoe, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	bold_segu = io.Fonts->AddFontFromMemoryTTF(&bold_segue, sizeof bold_segue, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	ico = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	ico_combo = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	ico_button = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	ico_grande = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 40, NULL, io.Fonts->GetGlyphRangesCyrillic());
//
//	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 15.0f, &icons_config, icons_ranges);
//
//	io.Fonts->AddFontDefault();
//
//	ImGuiStyle& style = ImGui::GetStyle();
//
//
//
//
//
//	ImGui_ImplWin32_Init(hWnd);
//	ImGui_ImplDX9_Init(p_Device);
//	return S_OK;
//}
//
//auto get_process_wnd(uint32_t pid) -> HWND
//{
//	std::pair<HWND, uint32_t> params = { 0, pid };
//	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
//		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
//		uint32_t processId = 0;
//
//		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
//			SetLastError((uint32_t)-1);
//			pParams->first = hwnd;
//			return FALSE;
//		}
//
//		return TRUE;
//
//		}, (LPARAM)&params);
//
//	if (!bResult && GetLastError() == -1 && params.first)
//		return params.first;
//
//	return NULL;
//}
//
//auto cleanup_d3d() -> void
//{
//	if (p_Device != NULL) {
//		p_Device->EndScene();
//		p_Device->Release();
//	}
//	if (p_Object != NULL) {
//		p_Object->Release();
//	}
//}
//
//auto set_window_target() -> void
//{
//	while (true) {
//		GameWnd = get_process_wnd(g_pid);
//		if (GameWnd) {
//			ZeroMemory(&GameRect, sizeof(GameRect));
//			GetWindowRect(GameWnd, &GameRect);
//			DWORD dwStyle = GetWindowLong(GameWnd, GWL_STYLE);
//			if (dwStyle & WS_BORDER)
//			{
//				GameRect.top += 32;
//				Height -= 39;
//			}
//			ScreenCenterX = Width / 2;
//			ScreenCenterY = Height / 2;
//			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
//		}
//	}
//}
//
//auto setup_window() -> void
//{
//	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_window_target, 0, 0, 0);
//	WNDCLASSEXA wcex = {
//		sizeof(WNDCLASSEXA),
//		0,
//		DefWindowProcA,
//		0,
//		0,
//		nullptr,
//		LoadIcon(nullptr, IDI_APPLICATION),
//		LoadCursor(nullptr, IDC_ARROW),
//		nullptr,
//		nullptr,
//		("Overlay"),
//		LoadIcon(nullptr, IDI_APPLICATION)
//	};
//
//	RECT Rect;
//	GetWindowRect(GetDesktopWindow(), &Rect);
//
//	RegisterClassExA(&wcex);
//
//	MyWnd = CreateWindowExA(NULL, ("Overlay"), ("Overlay"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
//	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED  | WS_EX_TOOLWINDOW);
//
//	SetLayeredWindowAttributes(MyWnd, 0, 255, LWA_COLORKEY);
//
//	MARGINS margin = { -1 };
//	DwmExtendFrameIntoClientArea(MyWnd, &margin);
//
//	ShowWindow(MyWnd, SW_SHOW);
//	UpdateWindow(MyWnd);
//}
