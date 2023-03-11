#include <iostream>
#include <windows.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "memory.h"
#include "hack.h"
#include "paint.h"
#include "offsets.h"

Hack::Hack() {}

Hack::Hack(Paint* paint) {
	this->paint = paint;
}

void Hack::Init() {
	DWORD procId = GetProcId(L"ac_client.exe");
	
	if (!procId) {
		std::cout << "Process not found" << std::endl;
	}

	client = GetModuleBaseAddress(procId, L"ac_client.exe");
	handle = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
}

void Hack::Update(int windowWidth, int windowHeight) {
	// Draw Lines
	uintptr_t Player = Memory::Read<uintptr_t>(handle, (client + offsets::oLocalPlayer));
	float PTeam = Memory::Read<uintptr_t>(handle, (Player + offsets::oTeam));
	ViewMatrix Matrix = Memory::Read<ViewMatrix>(handle, client + offsets::oViewMatrix);
	Vec3 PlayerOrigin = Memory::Read<Vec3>(handle, (Player + offsets::oCoordinateX));
	Direction PlayerDirection = Memory::Read<Direction>(handle, (Player + offsets::oYaw));
	for (int i = 1; i <= 16; ++i) {
		uintptr_t EntityList = Memory::Read<uintptr_t>(handle, (client + offsets::aEntityList));
		uintptr_t Entity = Memory::Read<uintptr_t>(handle, (EntityList + (i * 0x4)));
		if (Entity == NULL) continue;
		int Health = Memory::Read<int>(handle, (Entity + offsets::oHealth));
		if (Health <= 0) continue;
		float ETeam = Memory::Read<int>(handle, (Entity + offsets::oTeam));
		if (PTeam == ETeam) continue;
		PlayerName Name = Memory::Read<PlayerName>(handle, Entity + offsets::oName);
		Vec3 EntityOrigin = Memory::Read<Vec3>(handle, (Entity + offsets::oCoordinateX));
		Vec3 HeadOrigin = Memory::Read<Vec3>(handle, (Entity + 0x4));

		Vec2 FeetCoords;
		if (!WorldToScreen(EntityOrigin, FeetCoords, Matrix.Matrix, windowWidth, windowHeight)) continue;

		Vec2 HeadCoords;
		if (!WorldToScreen(HeadOrigin, HeadCoords, Matrix.Matrix, windowWidth, windowHeight)) continue;

		float entHeight = FeetCoords.y - HeadCoords.y;
		float entWidth = entHeight / 2.5f;

		// Choose Line Color
		D3DCOLOR color = D3DCOLOR_ARGB(50, 25, 25, 255);
		if (PTeam != ETeam) color = D3DCOLOR_ARGB(50, 255, 25, 25);

		// Draw FOV
		float cursorX = (windowWidth - 2) / 2;
		float cursorY = (windowHeight + 22) / 2;
		float radius = 32;
		paint->DrawCircle(cursorX, cursorY, radius, 8, D3DCOLOR_ARGB(255, 255, 255, 255));

		// Calculate Box
		float boxTop = HeadCoords.y - ((FeetCoords.y - HeadCoords.y) * 0.10);
		float boxBottom = FeetCoords.y;
		float boxLeft = FeetCoords.x - entWidth / 2;
		float boxRight = FeetCoords.x + entWidth / 2;

		// If Box touches FOV Highlight box
		float cursorTop = cursorY - radius;
		float cursorBottom = cursorY + radius;
		float cursorLeft = cursorX - radius;
		float cursorRight = cursorX + radius;
		if (
			PTeam != ETeam &&
			cursorY > boxTop &&
			cursorY < boxBottom &&
			cursorX > boxLeft &&
			cursorX < boxRight
			) {
			color = D3DCOLOR_ARGB(150, 255, 255, 25);
			// If Mouse Button Pressed, Aim At Head

			// calculate horizontal angle between enemy and player (yaw)
			float dx = EntityOrigin.x - PlayerOrigin.x;
			float dy = EntityOrigin.y - PlayerOrigin.y;
			float angleYaw = (atan2(dy, dx) * 180 / PI) + 90;

			// calculate vertical angle between enemy and player (pitch)
			float distance = sqrt(dx * dx + dy * dy);
			float dz = EntityOrigin.z - PlayerOrigin.z;
			float anglePitch = atan2(dz, distance) * 180 / PI;

			Memory::Write<float>(handle, (Player + offsets::oYaw), angleYaw);
			Memory::Write<float>(handle, (Player + offsets::oPitch), anglePitch);
		}

		// Draw Snap Line
		paint->DrawLine(windowWidth / 2, windowHeight, FeetCoords.x, FeetCoords.y, 1, color);

		// Draw Box
		paint->DrawLine(boxLeft, boxTop, boxRight, boxTop, 2, color);
		paint->DrawLine(boxLeft, boxBottom, boxRight, boxBottom, 2, color);
		paint->DrawLine(boxLeft, boxTop, boxLeft, boxBottom, 2, color);
		paint->DrawLine(boxRight, boxTop, boxRight, boxBottom, 2, color);
		
		// Draw Head Box
		D3DCOLOR headColor = D3DCOLOR_ARGB(50, 211, 96, 11);
		float headTop = HeadCoords.y - ((FeetCoords.y - HeadCoords.y) * 0.10);
		float headBottom = HeadCoords.y + ((FeetCoords.y - HeadCoords.y) * 0.10); 
		float headLeft = FeetCoords.x - entWidth / 4;
		float headRight = FeetCoords.x + entWidth / 4;
		paint->DrawLine(headLeft, headTop, headRight, headTop, 2, headColor);
		paint->DrawLine(headLeft, headBottom, headRight, headBottom, 2, headColor);
		paint->DrawLine(headLeft, headTop, headLeft, headBottom, 2, headColor);
		paint->DrawLine(headRight, headTop, headRight, headBottom, 2, headColor);

		//// Draw Health
		float remainingHealth = (FeetCoords.y - HeadCoords.y) * ((float)Health / 100);
		paint->DrawLine(FeetCoords.x + entWidth / 2, FeetCoords.y, FeetCoords.x + entWidth / 2, HeadCoords.y, 3, D3DCOLOR_ARGB(255, 255, 25, 25));
		paint->DrawLine(FeetCoords.x + entWidth / 2, FeetCoords.y, FeetCoords.x + entWidth / 2, FeetCoords.y - remainingHealth, 3, D3DCOLOR_ARGB(255, 25, 255, 25));

		// Draw Names
		paint->DrawText(Name.Name, FeetCoords.x, FeetCoords.y, 255, 255, 255, 255);

		//Memory::Write<float>(handle, (Entity + 0xC), -100);
	}	
}

bool Hack::WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	Vec4 clipCoords = {};

	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
	{
		return false;
	}

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return true;
}

