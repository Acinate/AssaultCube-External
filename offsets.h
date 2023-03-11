#pragma once

#include <windows.h>

namespace offsets {
	uintptr_t oLocalPlayer = 0x17E0A8;
	uintptr_t aEntityList = 0x18AC04;
	uintptr_t oViewMatrix = 0x17DFD0;
	// LOCATION
	unsigned int oCoordinateX = 0x28;
	unsigned int oCoordinateY = 0x2C;
	unsigned int oCoordinateZ = 0x30;
	// ATTRIBUTES
	uintptr_t oHealth = 0xEC;
	uintptr_t oRifleAmmo = 0x140;
	uintptr_t oTeam = 0x30C;
	uintptr_t oName = 0x205;
	uintptr_t oYaw = 0x34;
	uintptr_t oPitch = 0x38;
}