#include "sdk.h"
#include "overlay.h"
#include "menu.h"
#include <math.h>
#include <array>
#include <stdio.h>
#include <io.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <iostream>
#include <Wininet.h>
#include "xorstr.hpp"
#include <string>
#include <array>
#include "Header.h"
#include "auth1.hpp"
#include "main.h"
#include "GUI.h"
#pragma comment(lib, "wininet.lib")

RGBA espcolor;

struct FLinearColor {
	float R; 
	float G; 
	float B;
	float A;
};
auto ESPColor = ImColor(145, 0, 255);
RGBA green = { 145, 0, 255 };
RGBA black = { 145, 0, 255 };
using namespace SDK;

void esp()
{
	if (fov_draw)
	{
		auto draw = ImGui::GetBackgroundDrawList();
		draw->AddCircle(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2), fov, FtIM2(fov_color), 10000, 1.0f);
	}
	if (recoil_crosshair)
	{
		FMinimalViewInfo2 camera = read<FMinimalViewInfo2>(gcameramanager + 0x1FE0 + 0x10);
		auto ControlRotation = read<FVector>(glocalplayercontroller + CONTROL_ROT);
		float screen_width = GetSystemMetrics(SM_CXSCREEN);
		float screen_height = GetSystemMetrics(SM_CYSCREEN);

		FVector punchAngle = (camera.Rotation - ControlRotation);
		SDK::Clamp(punchAngle);
		int centerX = screen_width / 2;
		int centerY = screen_height / 2;

		int dx = screen_width / 115;
		int dy = screen_height / 115;

		centerX += (dx * (punchAngle.y));
		centerY -= (dy * (punchAngle.x));

		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(centerX, centerY), 5, FtIM2(crosshair_color), 10000, 0.5f);
	}	
	std::vector<EnemyPtr> actor = enemy_array;
	if (actor.empty()) {
		return;
	}

	for (int i = 0; i < actor.size(); i++)
	{
		EnemyPtr enemy = actor[i];

		Vector3 vBaseBone = SDK::GetBone(enemy.mesh_ptr, 0);
		Vector3 vBaseBoneOut = SDK::ProjectWorldToScreen(vBaseBone);
		Vector3 vBaseBoneOut2 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x, vBaseBone.y, vBaseBone.z - 15));
		Vector3 vHeadBone = SDK::GetBone(enemy.mesh_ptr, 8);
		Vector3 vHeadBoneOut = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z));
		Vector3 vHeadBoneOut2 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z));
		auto LocalRootComponent = read<uintptr_t>(glocalpawn + ROOT_COMPONENT);
		auto LocalRelativeLocation = read<Vector3>(LocalRootComponent + ROOT_POS);
		auto ActorRelativeLocation = read<Vector3>(enemy.root_component_ptr + ROOT_POS);
		auto distance = LocalRelativeLocation.Distance(ActorRelativeLocation);
		float BoxHeight = abs(vHeadBoneOut.y - vBaseBoneOut.y);
		float extra = abs(vHeadBoneOut.y - vBaseBoneOut.y) * 0.1;
		float BoxWidth = BoxHeight * 0.40;
		Vector3 camera_position = read<Vector3>(gcameramanager + CAMERA_POS);
		ImVec2 head_at_screen = ImVec2(vHeadBoneOut.x, vHeadBoneOut.y);
		float distance_modifier = camera_position.Distance(vHeadBone) * 0.001F;
		float Health = read<float>(enemy.damage_handler_ptr + HEALTH);
		float MaxHealth = read<float>(enemy.damage_handler_ptr + HEALTH + 0x4);
		auto BoneCount = enemy.bone_count;

		ULONG64 shooter_character_minimap = read<uint64_t>(enemy.actor_ptr + character_minimap_component);
		bool vischeck = read<byte>(shooter_character_minimap + local_visible);

		if (Health <= 0) continue;	
		if (distance <= 700) {
			if (map) {
				uint64_t minimap_component_1 = read<uint64_t>(enemy.actor_ptr + portrait_minimap_component);
				uint64_t minimap_component_2 = read<uint64_t>(enemy.actor_ptr + character_minimap_component);

				if (minimap_component_1 && minimap_component_2)
				{
					write<bool>(minimap_component_1 + local_observer, true);
					write<bool>(minimap_component_2 + local_observer, true);

					write<bool>(minimap_component_1 + local_visible, true);
					write<bool>(minimap_component_2 + local_visible, true);
				}
			}
			if (chams)
			{
				auto Outline3P = read<uintptr_t>(enemy.actor_ptr + outline_component);
				write<byte>(Outline3P + EnemyOutline, 1);
				//write<byte>(Outline3P + NeutralOutline, 1);
				//write<byte>(Outline3P + DeadEnemyOutline, 1);
				//write<FLinearColor>(driver.base + OUTLINE_TEAM_COLOR, { 0.00f, 1.00f, 0.00f, 3.00f });
				//write<FLinearColor>(driver.base + OUTLINE_ENEMY_COLOR, { 1.00f, 0.00f, 0.00f, 3.00f });
			}
			if (camerafov)
			{
				write<float>(gcameramanager + FOV_CHANGE, fov_float);
			}
			if (fresnel)
			{
				auto fresnel_comp = read<uintptr_t>(enemy.actor_ptr + fresnel_component);
				write<float>(fresnel_comp + fresnel_glow, 10000);
				//write<char>(fresnel_comp + fresnel_dirty, 0xff);
			}
			if (box3d)
			{
				ImColor health{ (uint8_t)min((510 * (100 - (int)(MaxHealth / Health * 100))) / 100, 255), (uint8_t)min((510 * (int)(MaxHealth / Health * 100)) / 100, 255), 0, 255 };
				Vector3 bottom1 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x + 40, vBaseBone.y - 40, vBaseBone.z));
				Vector3 bottom2 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x - 40, vBaseBone.y - 40, vBaseBone.z));
				Vector3 bottom3 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x - 40, vBaseBone.y + 40, vBaseBone.z));
				Vector3 bottom4 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x + 40, vBaseBone.y + 40, vBaseBone.z));

				Vector3 top1 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x + 40, vHeadBone.y - 40, vHeadBone.z + 15));
				Vector3 top2 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x - 40, vHeadBone.y - 40, vHeadBone.z + 15));
				Vector3 top3 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x - 40, vHeadBone.y + 40, vHeadBone.z + 15));
				Vector3 top4 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x + 40, vHeadBone.y + 40, vHeadBone.z + 15));

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(top1.x, top1.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(top2.x, top2.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(top3.x, top3.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(top4.x, top4.y), FtIM2(box_color), 2);

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(bottom2.x, bottom2.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(bottom3.x, bottom3.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(bottom4.x, bottom4.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(bottom1.x, bottom1.y), FtIM2(box_color), 2);

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(top1.x, top1.y), ImVec2(top2.x, top2.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(top2.x, top2.y), ImVec2(top3.x, top3.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(top3.x, top3.y), ImVec2(top4.x, top4.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(top4.x, top4.y), ImVec2(top1.x, top1.y), FtIM2(box_color), 2);
			}
			if (box2d)
			{

				Vector3 bottom1 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x + 40, vBaseBone.y - 0, vBaseBone.z));
				Vector3 bottom2 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x - 40, vBaseBone.y - 0, vBaseBone.z));
				Vector3 bottom3 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x - 40, vBaseBone.y + 0, vBaseBone.z));
				Vector3 bottom4 = SDK::ProjectWorldToScreen(Vector3(vBaseBone.x + 40, vBaseBone.y + 0, vBaseBone.z));

				Vector3 top1 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x + 40, vHeadBone.y - 0, vHeadBone.z + 15));
				Vector3 top2 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x - 40, vHeadBone.y - 0, vHeadBone.z + 15));
				Vector3 top3 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x - 40, vHeadBone.y + 0, vHeadBone.z + 15));
				Vector3 top4 = SDK::ProjectWorldToScreen(Vector3(vHeadBone.x + 40, vHeadBone.y + 0, vHeadBone.z + 15));

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(top1.x, top1.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(top2.x, top2.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(top3.x, top3.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(top4.x, top4.y), FtIM2(box_color), 2);

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(bottom2.x, bottom2.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(bottom3.x, bottom3.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(bottom4.x, bottom4.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(bottom1.x, bottom1.y), FtIM2(box_color), 2);

				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(top1.x, top1.y), ImVec2(top2.x, top2.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(top2.x, top2.y), ImVec2(top3.x, top3.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(top3.x, top3.y), ImVec2(top4.x, top4.y), FtIM2(box_color), 2);
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(top4.x, top4.y), ImVec2(top1.x, top1.y), FtIM2(box_color), 2);
			}
			if (head_circle)
			{
				ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(vHeadBoneOut.x, vHeadBoneOut.y), (7 / (distance * 0.001F)) / 25, FtIM2(head_color), 10000, 2);
			}
			if (bone_esp)
			{
				SDK::ProjectWorldToScreen, SDK::renderBones(enemy);
			}
			if (dist_esp)
			{
				char dist[64];
				sprintf_s(dist, skCrypt("[%.fm]"), distance);
				SDK::DrawString(14, vHeadBoneOut.x - 5, vHeadBoneOut.y - 13, &Col.white, true, false, dist);
			}
			if (health_bar)
			{
				SDK::DrawLeftProgressBar(vBaseBoneOut2.x - 8, vHeadBoneOut2.y - extra, BoxWidth, BoxHeight + extra, 3, Health);
			}
			if (snapline)
			{
				SDK::DrawLine(ImVec2(g_width / 2, g_height), ImVec2(vBaseBoneOut.x, vBaseBoneOut.y), FtIM2(snap_color), 1);
			}
			SDK::pRadar.x = (1920 / 2) + 680;
			SDK::pRadar.y = 1920 / 2 - 1080 / 2 - 400;
			if (rader)
			{
				ImGui::GetBackgroundDrawList()->AddRect(ImVec2(SDK::pRadar.x, SDK::pRadar.y), ImVec2(SDK::pRadar.x + 260, SDK::pRadar.y + 260), ImGui::ColorConvertFloat4ToU32(ImColor(141, 10, 242)), 3);
				auto radar_posX = SDK::pRadar.x + 130;
				auto radar_posY = SDK::pRadar.y + 130;
				DrawCircleFilled(radar_posX, radar_posY, 3, SDK::S4, 1000);
				Vector3 pos = read<Vector3>(enemy.root_component_ptr + ROOT_POS);
				SDK::DrawRadar(pos);
			}
			//if (ammo_esp) 
			//{
			//	auto Inventory = read<uintptr_t>(glocalpawn +inventory);

			//	printf(skCrypt("[>] Inventory: %llx\n"), Inventory);

			//	uintptr_t CurrentEquip = read<uintptr_t>(Inventory + currentweapon);

			//	printf(skCrypt("[>] CurrentEquip: %llx\n"), CurrentEquip);

			//	int weaponObj = read<int>(CurrentEquip + fnameid);

			//	printf(skCrypt("[>] weaponObj: %d\n"), weaponObj);

			//	uintptr_t MagazineAmmo = read<uintptr_t>(CurrentEquip + magazineinfo);

			//	printf(skCrypt("[>] MagazineAmmo: %llx\n"), MagazineAmmo);

			//	uint32_t AuthResourceAmount = read<uint32_t>(MagazineAmmo + currentammo);

			//	printf(skCrypt("[>] AuthResourceAmount: %llx\n"), AuthResourceAmount);

			//	uint32_t MaxAmmo = read<uint32_t>(MagazineAmmo + maxammo);

			//	printf(skCrypt("[>] MaxAmmo: %llx\n"), MaxAmmo);

			//	std::string DisplayAmmo = "   A: " + std::to_string(AuthResourceAmount) + " / " + std::to_string(MaxAmmo);

			//	SDK::DrawString(14, vHeadBoneOut.x - 5, vHeadBoneOut.y - 13, &Col.white, true, false, DisplayAmmo.c_str());
			//}
		}
	}
}

void aimbot()
{
	try
	{
		if (0x02)
		{
			int hitboxid;
			int dwIndex = SDK::ActorLoop();
			if (dwIndex < 0) return;
			if (Bones == 1)
				hitboxid = 8;
			else if (Bones == 2)
				hitboxid = 7;
			else if (Bones == 3)
				hitboxid = 5;
			if (GetAsyncKeyState(WallHackKey))
			{
				EnemyPtr enemy = enemy_array[dwIndex];
				FMinimalViewInfo2 camera = read<FMinimalViewInfo2>(gcameramanager + 0x1FE0 + 0x10);
				FVector neck_position = SDK::GetBone2(enemy.mesh_ptr, hitboxid);
				FVector root_position = read<FVector>(enemy.root_component_ptr + ROOT_POS);
				//auto character_portraint_minimap = read<uintptr_t>(enemy.actor_ptr + 0x1180);
				//auto visible = read<bool>(character_portraint_minimap + 0x501);
				if (neck_position.z <= root_position.z)
				{
					return;
				}
				FVector localView = read<FVector>(glocalplayercontroller + CONTROL_ROT);
				FVector vecCaclculatedAngles = SDK::fhgfsdhkfshdghfsd205(camera.Location, neck_position);
				FVector angleEx = SDK::CaadadalcAngle(camera.Location, neck_position);
				FVector fin = FVector(vecCaclculatedAngles.y, angleEx.y, 0);
				float Health = read<float>(enemy.damage_handler_ptr + HEALTH);
				if (Health >= 0) {
					if (rcs) {
						FVector delta = fin - localView;
						FVector TargetAngle = localView + delta;
						SDK::Clamp(TargetAngle);
						SDK::RCS(TargetAngle, camera.Rotation, smooth);
						float temp = SDK::GetFov(camera.Rotation, vecCaclculatedAngles);
					}
					else {
						FVector delta = fin - localView;
						SDK::NormalizeAngles(delta);
						FVector TargetAngle = localView + (delta / smooth);
						SDK::NormalizeAngles(TargetAngle);
						write<FVector>(glocalplayercontroller + CONTROL_ROT, TargetAngle);
						float temp = SDK::GetFov(camera.Rotation, vecCaclculatedAngles);
					}
				}
			}
		}
	}
	catch (std::exception e)
	{

	}
}

void cache()
{
	while (true)
	{
		//system("cls");
		uintptr_t UWorld = read_guarded<uintptr_t>(driver.guarded + UWORLD);
		uintptr_t GameInstance = read_guarded<uintptr_t>(UWorld + GAME_INSTANCE);
		uintptr_t PersistentLevel = read_guarded<uintptr_t>(UWorld + PERSISTENT_LEVEL);
		uintptr_t LocalPlayers_Array = read_guarded<uintptr_t>(GameInstance + LOCALPLAYERS_ARRAY);
		uintptr_t LocalPlayer = read_guarded<uintptr_t>(LocalPlayers_Array);
		glocalplayercontroller = read_guarded<uintptr_t>(LocalPlayer + PLAYER_CONTROLER);
		glocalpawn = read_guarded<uintptr_t>(glocalplayercontroller + LOCALPLAYERPAWN);
		glocaldamagehandler = read_guarded<uintptr_t>(glocalpawn + DAMAGE_HANDLER);
		uintptr_t LocalPlayerState = read_guarded<uintptr_t>(glocalpawn + PLAYER_STATE);
		uintptr_t LocalTeamComponent = read_guarded<uintptr_t>(LocalPlayerState + TEAM_COMP);
		glocalteamid = read_guarded<int>(LocalTeamComponent + TEAM_ID);
		gcameramanager = read_guarded<uintptr_t>(glocalplayercontroller + CAMERA_MANAGER);
		uintptr_t ActorArray = read_guarded<uintptr_t>(PersistentLevel + ACTOR_ARRAY);
		int ActorCount = read_guarded<int>(PersistentLevel + ACTOR_COUNT);

		//printf(skCrypt("[>] UWorld: %p\n"), UWorld);
		//printf(skCrypt("[>] GameInstance: %p\n"), GameInstance);
		//printf(skCrypt("[>] PersistentLevel: %p\n"), PersistentLevel);
		//printf(skCrypt("[>] LocalPlayers_Array: %p\n"), LocalPlayers_Array);
		//printf(skCrypt("[>] LocalPlayer: %p\n"), LocalPlayer);
		//printf(skCrypt("[>] glocalplayercontroller: %p\n"), glocalplayercontroller);
		//printf(skCrypt("[>] glocalpawn: %p\n"), glocalpawn);
		//printf(skCrypt("[>] glocaldamagehandler: %p\n"), glocaldamagehandler);
		//printf(skCrypt("[>] LocalPlayerState: %p\n"), LocalPlayerState);
		//printf(skCrypt("[>] LocalTeamComponent: %p\n"), LocalTeamComponent);
		//printf(skCrypt("[>] glocalteamid: %d\n"), glocalteamid);
		//printf(skCrypt("[>] gcameramanager: %p\n"), gcameramanager);
		//printf(skCrypt("[>] ActorArray: %p\n"), ActorArray);
		//printf(skCrypt("[>] ActorCount: %d\n"), ActorCount);

		enemy_array = SDK::GetEnemyArray(ActorArray, ActorCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}

using namespace KeyAuth;
std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
std::string name = skCrypt("").decrypt(); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = skCrypt("").decrypt(); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = skCrypt("").decrypt(); // app secret, the blurred text on licenses tab and other tabs
std::string version = skCrypt("1.0").decrypt(); // leave alone unless you've changed version on website
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting
std::string key;
api KeyAuthApp(name, ownerid, secret, version, url);
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);

int main()
{
	system("move BMDOHYEON_ttf.ttf C:\\");
	system("cls");
	std::string consoleTitle = (std::string)skCrypt("SuperShell - Built at:  ") + compilation_date + " " + compilation_time;
	SetConsoleTitleA(consoleTitle.c_str());
	KeyAuthApp.init();
	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}
	int option = 4;
	std::string username;
	std::string password;
	std::string key;
	switch (option)
	{
	case 1:
		std::cout << skCrypt("\n\n Enter username: ");
		std::cin >> username;
		std::cout << skCrypt("\n Enter password: ");
		std::cin >> password;
		KeyAuthApp.login(username, password);
		break;
	case 2:
		std::cout << skCrypt("\n\n Enter username: ");
		std::cin >> username;
		std::cout << skCrypt("\n Enter password: ");
		std::cin >> password;
		std::cout << skCrypt("\n Enter license: ");
		std::cin >> key;
		KeyAuthApp.regstr(username, password, key);
		break;
	case 3:
		std::cout << skCrypt("\n\n Enter username: ");
		std::cin >> username;
		std::cout << skCrypt("\n Enter license: ");
		std::cin >> key;
		KeyAuthApp.upgrade(username, key);
		break;
	case 4:
		std::cout << skCrypt("\n Enter license: ");
		std::cin >> key;
		KeyAuthApp.license(key);
		break;
	default:
		std::cout << skCrypt("\n\n Status: Failure: Invalid Selection");
		Sleep(3000);
		exit(0);
	}

	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}

	std::cout << skCrypt("\n User data:");
	std::cout << skCrypt("\n Username: ") << KeyAuthApp.data.username;
	std::cout << skCrypt("\n IP address: ") << KeyAuthApp.data.ip;
	std::cout << skCrypt("\n Hardware-Id: ") << KeyAuthApp.data.hwid;
	std::cout << skCrypt("\n Create date: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.createdate)));
	std::cout << skCrypt("\n Last login: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin)));
	std::cout << skCrypt("\n Subscription(s): ");

	for (int i = 0; i < KeyAuthApp.data.subscriptions.size(); i++) {
		auto sub = KeyAuthApp.data.subscriptions.at(i);
		std::cout << skCrypt("\n name: ") << sub.name;
		std::cout << skCrypt(" : expiry: ") << tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry)));
	}

	printf(skCrypt("Thanks for using SuperShell !"));
	Sleep(2000);
	init();
	if (!driver.get_guarded_base()) { LoadDriver(); }
	system(skCrypt("Color 2"));
	std::cin.clear();
	
	if (Entryhwnd == NULL)
	{
		printf(skCrypt("open valorant...\n"));
	}
	while (Entryhwnd == NULL)
	{
		g_pid = SDK::GetVALORANTPID();
		driver.base = driver.get_process_base(g_pid);
		Entryhwnd = get_process_wnd(g_pid);
		Sleep(1);
	}
	driver.attach(g_pid);
	driver.guarded = driver.get_guarded_base();

	if (g_pid)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)D3D::GUIThread, 0, 0, 0);
	}

	//std::cout << skCrypt("[+] Base Address: 0x") << std::hex << driver.base << std::endl;
	//std::cout << skCrypt("[+] Guarded Base: 0x") << std::hex << driver.guarded << std::endl;

	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)cache, nullptr, NULL, nullptr);
	system("cls");
	printf(skCrypt("Successfully Completed!\n"));
	printf(skCrypt("Thanks for using SuperShell !"));


	while (g_pid)
	{
		if (GetAsyncKeyState(VK_F2) & 0x8001)
			system("cls");
		bool bPress = false;

		while (GetAsyncKeyState(VK_HOME) & 0x8000 || GetAsyncKeyState(VK_F6) & 0x8000)
		{
			if (!bPress && D3D::bWindowTop)
			{
				Window::Visible ^= 1;
				bPress = true;
			}

			Sleep(100);
		}

		if ((GetAsyncKeyState(VK_END) & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState(VK_F8) & 0x8000))
			exit(1);
		Sleep(100);
	}
}

std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}