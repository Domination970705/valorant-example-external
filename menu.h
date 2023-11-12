#pragma once
#include "overlay.h"
#include <string>
#include <Windows.h>
#include "settings.h"
#include "icons.h"
#include "Images.h"

int currentColumn = 1;
static bool showing = true;
HWND hwnd = GetDesktopWindow();
int WallHackKey = 6;
int WallHackKeyStatus;
#include "imgui/imgui_internal.h"
#include <random>
const char* AllBones[] = { "Head", "Neck","Body"};
static std::string random_string(int length)
{
	std::string str(("77432651029300129834675012934978659123446898905478065625894396234"));
	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(str.begin(), str.end(), generator);
	return str.substr(0, length);    // assumes 32 < number of characters in str         
}
static bool animate = false;
static bool giris = false;
static bool giris2 = false;
static bool infov = false;
bool hideonads = false;
bool GirisTuneli = false;
float size_child = 0;
bool active = false;
int clicked = 0;

static DWORDLONG random_number(DWORDLONG min, DWORDLONG max)
{
	srand(time(0));
	DWORDLONG out = min + rand() % (max - min + 1);
	return out;
}
static std::string randomclassname = "                          omnitrix ID " + random_string(random_number(10, 30));
ImVec4 to_vec4(float r, float g, float b, float a)
{
	return ImVec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}
bool loader_active = true;
namespace feats
{
	static bool box_color_edit = false;
	static bool head_color_edit = false;
	static bool snap_color_edit = false;
	static bool fov_color_edit = false;
	static bool bone_color_edit = false;
	static float box_color[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
	static float box_visible_color[4] = { 1.00f, 1.00f, 1.00f, 1.00f };
	static float head_color[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
	static float snap_color[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
	static float fov_color[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
	static float bone_color[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
	static float crosshair_color[4] = { 3.10f, 0.09f, 2.12f, 1.00f };
	static bool aimtab = true;
	static bool esptab = false;
	static bool misctab = false;
	static bool HeadB = true;
	static bool NeckB = false;
	static bool BodyB = false;
	static bool glow = false;
	static bool rcs = false;
	static bool silent = false;
	static bool chams = false;
	static bool map = false;
	static bool camerafov = false;
	static bool fov_draw = true;
	static bool recoil_crosshair = true;
	static bool fresnel = false;
	static bool box3d = true;
	static bool box2d = false;
	static bool bone_esp = false;
	static bool dist_esp = false;
	static bool health_bar = false;
	static bool snapline = false;
	static bool rader = true;
	static bool head_circle = true;
	static bool ammo_esp = true;
	static int Bones = 1;
	static float fov_float = 110;	
	static int glowamount = 10000;
	static int smooth = 11;
	static int fov = 50;
}
using namespace feats;

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
static const char* keyNames[] = {
	"",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};
void WallHackChangeKey(void* blank)
{
	WallHackKeyStatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				WallHackKey = i;
				WallHackKeyStatus = 0;
				return;
			}
		}
	}
}
void WallHackKeyButton(int WallKey, void* changekey, int status, ImVec2 a)
{
	const char* preview_value = NULL;
	if (WallHackKey >= 0 && WallHackKey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, WallHackKey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = skCrypt("Select A Key");
	else
		aimkeys = preview_value;
	if (status == 1)
	{
		aimkeys = skCrypt("Press A key");
	}
	if (ImGui::Button(aimkeys.c_str(), a))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}
void draw_menu()
{

	DWORD window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImColor(0, 255, 132);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.30f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 0.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 0.00f);
	colors[ImGuiCol_CheckMark] = ImColor(58, 214, 133);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.114f, 0.114f, 0.114f, 0.400f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_Button] = ImColor (0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ButtonActive] = ImColor(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ButtonHovered] = ImColor(0.06f, 0.06f, 0.06f, 0.94f);
	style.FrameRounding = 5;
	style.PopupRounding = 7.f;
	ImGui::StyleColorsDark();
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		showing = !showing;
	}
	if (showing)
	{
		ImGui::Begin(randomclassname.c_str(), &loader_active, window_flags);
		{

                /*if (ImGui::BeginTabBar("Aim", currentColumn == 1)) currentColumn = 1;
                ImGui::SameLine();
				
                if (currentColumn == 1)
                {
                    ImGui::BeginChild("dsxsfdfssor", ImVec2(537, 298), true);
                    {
						ImGui::Text("Aim Keybind"); ImGui::SameLine(); WallHackKeyButton(WallHackKey, WallHackChangeKey, WallHackKeyStatus, ImVec2(125, 22));
                        ImGui::SliderInt("Fov", &Aim_Bot_FOV_X_Int, 35, 1000, "% 2f");

                        ImGui::SliderInt("Smooth", &Aim_Bot_Smooth_Level_Int, 1, 20, "%2f");

						ImGui::Checkbox("map", &map);

						ImGui::Checkbox("chams", &chams);

						ImGui::Checkbox("Camera Fov", &fov);

						ImGui::SliderInt("Camera Fov Value", &fov_int, 70, 170, "% 2f");

                        if (HeadB == false & NeckB == false & BodyB == false)
                        {
                            if (Bones == 1)
                            {
                                HeadB = true;
                            }

                            if (Bones == 2)
                            {
                                NeckB = true;
                            }

                            if (Bones == 3)
                            {
                                BodyB = true;
                            }
                        }

                        if (ImGui::Checkbox("Head", &HeadB))
                        {
                            BodyB = false;
                            NeckB = false;
                        }

                        if (HeadB == true)
                        {
                            Bones = 1;
                        }



                        if (ImGui::Checkbox("Neck", &NeckB))
                        {
                            BodyB = false;
                            HeadB = false;
                        }

                        if (NeckB == true)
                        {
                            Bones = 2;
                        }

                        if (ImGui::Checkbox("Body", &BodyB))
                        {
                            NeckB = false;
                            HeadB = false;
                        }

                        if (BodyB == true)
                        {
                            Bones = 3;
                        }

                    }ImGui::EndChild();
                }*/
		}
	}
}
