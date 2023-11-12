#pragma once
#include <d3d11.h>
#include <dwmapi.h>


#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "VkKey.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <cstring>
#include "settings.h"
#include "main.h"
#pragma warning(disable:4996)
#pragma comment(lib, "d3d11.lib")

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "urlmon.lib")

DWORD ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
DWORD ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
RECT rect;



//void EntityRender();
void Loadpng();
void SkillRender();
void FovRender();

void Esp();





auto get_process_wnd(uint32_t pid) -> HWND
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
		uint32_t processId = 0;

		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
			SetLastError((uint32_t)-1);
			pParams->first = hwnd;
			return FALSE;
		}

		return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}

namespace Window
{
	bool Visible = false;
	bool bset = true;
	HWND twnd;
	void CreateStyle()
	{
		// Setup style
		{
			ImGuiStyle* style = &ImGui::GetStyle();

			style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
			style->WindowRounding = 5.0f;
			style->FrameRounding = 4.0f;
			style->IndentSpacing = 25.0f;
			style->ScrollbarSize = 15.0f;
			style->ScrollbarRounding = 9.0f;
			style->GrabMinSize = 5.0f;
			style->GrabRounding = 3.0f;
			style->TabRounding = 2.5f;

			ImVec4* colors = ImGui::GetStyle().Colors;

			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // 글자
			colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.70f); //메인배경
			colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.16f, 0.10f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.16f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.16f, 0.18f, 1.00f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.18f, 0.20f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f); //제목 배경
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f); // 제목 건들였을때
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15f, 0.16f, 0.18f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 255.0f, 72.0f, 1.0f); // 체크표시
			colors[ImGuiCol_SliderGrab] = ImVec4(0.84f, 0.06f, 0.22f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.08f, 0.24f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.86f, 0.08f, 0.24f, 1.00f); // 버튼
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.90f, 0.12f, 0.27f, 1.00f);// 버튼 클릭
			colors[ImGuiCol_ButtonActive] = ImVec4(0.95f, 0.15f, 0.30f, 1.00f);// 버튼 홀딩
			colors[ImGuiCol_Header] = ImVec4(0.86f, 0.08f, 0.24f, 1.00f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.88f, 0.10f, 0.25f, 1.00f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.90f, 0.12f, 0.27f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_Tab] = ImVec4(0.83f, 0.05f, 0.21f, 0.90f);
			colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.f, 0.f, 1.00f);
			colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.f, 0.f, 1.00f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		}
	}

	void Initialize(ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pd3dDeviceContext, HWND hWnd)
	{


		//밑에가 원래
		twnd = Entryhwnd;


		ImGui::CreateContext();


		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.IniFilename = NULL;
		//io.IniSavingRate = 0;
		io.Fonts->AddFontFromFileTTF(("C:\\BMDOHYEON_ttf.ttf"), 15.f, NULL, io.Fonts->GetGlyphRangesKorean()) || io.Fonts->AddFontFromFileTTF(("C:\\asset\\BMDOHYEON_ttf.ttf"), 15.f, NULL, io.Fonts->GetGlyphRangesKorean());
		CreateStyle();

		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

		twnd = Entryhwnd;


		//Config::baseY = ScreenHeight / 3;
		Sleep(500);

	}

	void Menu(HWND hwnd)
	{
		ImDrawList* list = ImGui::GetForegroundDrawList();
		/*
		ImGui::PushFont(light_font);
		list->AddText(ImVec2(10, 20), ImGui::GetColorU32(ImVec4(1, 1, 1, 1)), xorstr("Undefined"));
		*/

		//EntityRender();
		//printf("%d %d %d %d\n", rect.left, rect.top, rect.right, rect.bottom);


		if (!Visible) {
			if (!bset) {
				bset = true;
				//SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				SetWindowPos(twnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);

			}
			return;
		}
		else if (Visible && bset) {
			bset = false;
			long style = 0;
			style &= ~(WS_VISIBLE);
			style |= WS_EX_TOOLWINDOW;
			style &= ~(WS_EX_APPWINDOW);
			SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
		}


		ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(500, 400), 0); // ImGui::SetNextWindowSize(ImVec2(850, 500), 0);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.8f });

		if (ImGui::Begin(("SuperShell"), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings));
		{
			if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem(" Aimbot "))
				{
					ImGui::Checkbox("RCS", &rcs);
					ImGui::Checkbox("Fov Draw", &fov_draw);
					ImGui::Checkbox("Recoil Crosshair", &recoil_crosshair);
					ImGui::Hotkey(("Aim Keybind"), &WallHackKey);
					ImGui::SliderInt("Fov", &fov, 35, 1000);
					ImGui::SliderInt("Smooth", &smooth, 1, 20);

					if (ImGui::Checkbox("Head", &HeadB))
					{
						NeckB = false;
						BodyB = false;
						Bones = 1;
					}
					if (ImGui::Checkbox("Neck", &NeckB))
					{
						HeadB = false;
						BodyB = false;
						Bones = 2;
					}
					if (ImGui::Checkbox("Body", &BodyB))
					{
						HeadB = false;
						NeckB = false;
						Bones = 3;
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(" Visual "))
				{
					if (ImGui::Checkbox("3D box esp", &box3d))
					{
						box2d = false;
					}
					if (ImGui::Checkbox("2D box esp", &box2d))
					{
						box3d = false;
					}
					ImGui::Checkbox("snapline esp", &snapline);
					ImGui::Checkbox("rader esp", &rader);
					ImGui::Checkbox("ammo esp", &ammo_esp);
					ImGui::Checkbox("health bar esp", &health_bar);
					ImGui::Checkbox("distance esp", &dist_esp);
					ImGui::Checkbox("head circle esp", &head_circle);
					ImGui::Checkbox("bone esp", &bone_esp);
					ImGui::Checkbox("map esp(risk)", &map);
					ImGui::Checkbox("chams(risk, fixing)", &chams);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(" Misc "))
				{
					ImGui::BulletText("Box Color");
					ImGui::SameLine();
					ImGui::ColorEdit4(("Box Color"), box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

					ImGui::BulletText("Head Circle Color");
					ImGui::SameLine();
					ImGui::ColorEdit4(("Head Circle Color"), head_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

					ImGui::BulletText("Snapline Color");
					ImGui::SameLine();
					ImGui::ColorEdit4(("Snapline Color"), snap_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

					ImGui::BulletText("Fov Color");
					ImGui::SameLine();
					ImGui::ColorEdit4(("Fov Color"), fov_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

					ImGui::BulletText("Bone Color");
					ImGui::SameLine();
					ImGui::ColorEdit4(("Bone Color"), bone_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

					ImGui::BulletText("Recoil Crosshair Color");
					ImGui::SameLine();
					ImGui::ColorEdit4(("Recoil Crosshair Color"), crosshair_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

					ImGui::Checkbox("Change Camera Fov(risk)", &camerafov);
					if (camerafov) {
						ImGui::SliderFloat("Camera Fov Value", &fov_float , 70, 170);
					}
					ImGui::Checkbox("outline glow(risk)", &fresnel);
					if (fresnel)
					{
						ImGui::SliderInt("outline glow", &glowamount, 100, 1000);
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}
	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace D3D
{


	DWORD Width, Height;

	HWND hwnd = 0;
	WNDCLASSEX wc;

	//d3d data
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool Initialize(HWND& hWnd);
	bool CreateD3D(HWND hWnd1);
	void CleanupRenderTarget();
	void CreateRenderTarget();
	void CleanupDeviceD3D();

	bool bWindowTop = false;

	DWORD WINAPI GUIThread(PVOID)
	{
		Initialize(hwnd);
		//SetWindowDisplayAffinity(hwnd, 1);

		HWND tWnd = Entryhwnd;

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (get_process_wnd(g_pid))
		{
			DWORD Style = GetWindowLong(tWnd, GWL_STYLE);

			GetWindowRect(tWnd, &rect);



			if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				continue;
			}


			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame();
			esp();
			aimbot();
			if (tWnd == GetForegroundWindow() || hwnd == GetForegroundWindow()) {
				bWindowTop = true;

				if (Window::Visible)
					SetForegroundWindow(hwnd);

				SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_NOZORDER);
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);


				Window::Menu(hwnd);

			}
			else
			{
				bWindowTop = false;
				SetWindowPos(hwnd, HWND_BOTTOM, 1, 1, 0, 0, 0);
			}

			ImGui::Render();



			ImVec4 v1_ = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			pContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
			pContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&v1_);

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			pSwapChain->Present(1, 0);
			//pSwapChain->Present(0, 0);

			//Sleep(1);
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
		::DestroyWindow(hwnd);
		::UnregisterClass(wc.lpszClassName, wc.hInstance);

		return 0;
	}

	bool Initialize(HWND& hwnd)
	{
		wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, *(HINSTANCE*)(__readgsqword(0x60) + 0x10), NULL, NULL, NULL, NULL, ("davn48"), NULL };

		RegisterClassEx(&wc);
		//hwnd = CreateWindow(wc.lpszClassName, "", WS_POPUP, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
		hwnd = CreateWindowExA(NULL, ("davn48"), (LPCSTR)NULL, WS_POPUP, 1, 1, 1, 1, (HWND)NULL, (HMENU)NULL, wc.hInstance, (void*)NULL);


		if (!CreateD3D(hwnd))
		{
			CleanupDeviceD3D();
			UnregisterClass(wc.lpszClassName, wc.hInstance);
			return false;
		}

		MARGINS margins = { -1 };
		DwmExtendFrameIntoClientArea(hwnd, &margins);

		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_NOACTIVATE);
		SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);


		ShowWindow(hwnd, SW_SHOWDEFAULT);
		//ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hwnd);

		//위는새코드
		Window::Initialize(pDevice, pContext, hwnd);


		return true;
	}

	bool Uninitialize()
	{
		HWND hWnd = Entryhwnd;

		//DetourTransactionBegin();
		//DetourUpdateThread(GetCurrentThread());
		//if (DetourDetach(&(PVOID&)phookD3D11Present, (PBYTE)hkD3D11Present) != NO_ERROR)
		//	MessageBox(hWnd, xorstr("Failed to Detours"), xorstr("Error"), MB_ICONERROR);
		//if (DetourDetach(&(PVOID&)phookD3D11ResizeBuffer, (PBYTE)hookD3D11ResizeBuffers) != NO_ERROR)
		//	MessageBox(hWnd, xorstr("Failed to Detours"), xorstr("Error"), MB_ICONERROR);
		//DetourTransactionCommit();
		return true;
	}

	bool CreateD3D(HWND hWnd1)
	{


		//D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL featureLevel;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = 0;
		swapChainDesc.BufferDesc.Height = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.OutputWindow = hWnd1;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE; //((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
		//swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevelArray, 2
			, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext) != S_OK)
			return false;


		CreateRenderTarget();

		return true;
	}

	void CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
		pBackBuffer->Release();

		D3D11_TEXTURE2D_DESC desc;
		pBackBuffer->GetDesc(&desc);
		Width = desc.Width;
		Height = desc.Height;

	}

	void CleanupRenderTarget()
	{
		if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
	}

	void CleanupDeviceD3D()
	{
		CleanupRenderTarget();
		if (pSwapChain) { pSwapChain->Release(); pSwapChain = NULL; }
		if (pContext) { pContext->Release(); pContext = NULL; }
		if (pDevice) { pDevice->Release(); pDevice = NULL; }
	}

	
	void KeyHook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (!settings::getkey) return;
		int nkey = 0;


		for (int i = 0; i < VKEY_NAMES_EN.size(); i++)
		{
			if (GetAsyncKeyState(VKEY_NAMES_EN[i].second) & 0x8001)
			{
				nkey = VKEY_NAMES_EN[i].second;
			}
		}

		if (nkey)
		{
			if (settings::getkey)
			{
				settings::key[settings::hotkey] = nkey;
				settings::getkey = false;
			}
		}
	}

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		KeyHook(hWnd, msg, wParam, lParam);

		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (pDevice != NULL && wParam != SIZE_MINIMIZED)
			{
				CleanupRenderTarget();
				pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
				CreateRenderTarget();
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

}

ImVec4 HSVToRGB(float health)
{
	ImVec4 result;
	if (health >= 0.5)
	{
		result.x = (1.0f - health) * 2;
		result.y = 1.0f;
		result.z = 0.0f;
		result.w = 1;
	}
	else
	{
		result.x = 1.0f;
		result.y = health;
		result.z = 0.0f;
		result.w = 1;

	}
	return result;
}
ImFont* m_pFont{};

float dx11_text(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
{
	ImDrawList* window = ImGui::GetBackgroundDrawList();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	m_pFont = ImGui::GetFont();

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (getline(stream, line))
	{
		ImVec2 textSize = m_pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			window->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

			window->AddText(m_pFont, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
		}
		else
		{
			window->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

			window->AddText(m_pFont, size, { position.x, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
		}

		y = position.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}




void DrawEdges(const ImVec2& top, const ImVec2& bot, const ImVec2& base, ImU32 col)
{
	ImDrawList* Draw = ImGui::GetForegroundDrawList();
	float scale = (bot.y - top.y) / 3.0f;
	float length = scale / 2;
	Draw->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale + length, top.y), col, 2.0f); //  --- Top left
	Draw->AddLine(ImVec2(base.x - scale, top.y), ImVec2(base.x - scale, top.y + length), col, 2.0f); // | Top left
	Draw->AddLine(ImVec2(base.x + scale / 3, top.y), ImVec2(base.x + scale / 3 + length, top.y), col, 2.0f); // --- Top right
	Draw->AddLine(ImVec2(base.x + scale / 3 + length, top.y), ImVec2(base.x + scale / 3 + length, top.y + length), col, 2.0f); // | Top right
	Draw->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale + length, bot.y), col, 2.0f); // --- Bottom left
	Draw->AddLine(ImVec2(base.x - scale, bot.y), ImVec2(base.x - scale, bot.y - length), col, 2.0f); // | Bottom left
	Draw->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3, bot.y), col, 2.0f); // --- Bottom right
	Draw->AddLine(ImVec2(base.x + scale / 3 + length, bot.y), ImVec2(base.x + scale / 3 + length, bot.y - length), col, 2.0f); // | Bottom right
}



static void RenderCorneredBox(const Vector2& from, const Vector2& to, uint32_t color, float thickness = 1.f)
{

	int X = from.x, Y = from.y, W = to.x, H = to.y;
	float lineW = (W / 3);
	float lineH = (H / 3);
	ImDrawList* window = ImGui::GetBackgroundDrawList();

	window->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 1);
	window->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 1);
	window->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 1);
	window->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 1);
	window->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 1);
	window->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 1);
	window->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 1);
	window->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 1);

	window->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	window->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	window->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	window->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	window->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	window->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	window->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	window->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

void RenderFilledRect(const Vector2& from, const Vector2& to, uint32_t color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All)
{
	int X = from.x, Y = from.y, W = to.x, H = to.y;
	ImDrawList* window = ImGui::GetBackgroundDrawList();

	auto a = (color >> 24) & 0xFF;
	auto r = (color >> 16) & 0xFF;
	auto g = (color >> 8) & 0xFF;
	auto b = (color) & 0xFF;

	window->AddRectFilled(ImVec2(from.x, from.y), ImVec2(from.x + W, from.y + H), ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), NULL, NULL);
}