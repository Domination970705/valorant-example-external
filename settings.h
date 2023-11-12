#pragma once
#include "struct.h"
#include "skCrypt.h"
uintptr_t g_pid;
uintptr_t g_base;
HWND Entryhwnd = NULL;
static inline char* memdup(const char* s, size_t n)
{
	char* t = (char*)malloc(n);
	memcpy(t, s, n);
	return t;
}
#define _memdup(object) memdup(object, sizeof(object))
namespace settings{
	bool show_menu = true;
	bool getkey = false;
	bool esp_enabled = false;
	int hotkey = 0;
	int key[5] = { 1, 1, 1, 1, 1 };
	bool bone_esp = false;
	bool headcircle = false;
	bool box_esp = false;
	bool snapline = false;
	bool distance = false;
	bool healthbar = false;
	bool silentaim = true;
	bool glow = false;
	int box_type, line_type;
	const char* box[] = { skCrypt("Corner Box"), skCrypt("Normal Box"), skCrypt("3D Box")};
	const char* line[] = { skCrypt("Middle Down"), skCrypt("Middle Top"), skCrypt("Crosshair")};
	int aimkey = 6;
	bool aimbot = true;
	bool visiblecheck = true;
	bool teamcheck;
	bool rcs;
	bool autoshoot = false;
	float fov = 60;
	float smooth = 0;
	int hitbox;
	const char* hitbox2[] = { skCrypt("Head"), skCrypt("Neck"), skCrypt("Body")};
}

namespace Settings {
	static int Tab = 1;
	static int malyusuf = 1;
	static int Renk2 = 1;
	static int Renk3 = 1;
	static int Renk4 = 1;
}