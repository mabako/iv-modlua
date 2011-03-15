//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: InterfaceCommon.h
//======================================================================

#pragma once

#include <math.h>

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#else
#include <string.h>
typedef bool BOOL;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef char *PCHAR;
typedef void *PVOID;
typedef unsigned char BYTE;

#define TRUE (1)
#define FALSE (0)
#endif

typedef unsigned short EntityId;

struct Vector3
{
	float X;
	float Y;
	float Z;

	Vector3()
	{
		X = Y = Z = 0;
	}

	Vector3(float fX, float fY, float fZ)
	{
		X = fX; Y = fY; Z = fZ;
	}

	bool IsEmpty() const
	{
		return (X == 0 && Y == 0 && Z == 0);
	}

	float Length() const
	{
		return sqrt((X * X) + (Y * Y) + (Z * Z));
	}

	Vector3 operator+ (const Vector3& vecRight) const
	{
		return Vector3(X + vecRight.X, Y + vecRight.Y, Z + vecRight.Z);
	}

	Vector3 operator+ (float fRight) const
	{
		return Vector3(X + fRight, Y + fRight, Z + fRight);
	}

	Vector3 operator- (const Vector3& vecRight) const
	{
		return Vector3(X - vecRight.X, Y - vecRight.Y, Z - vecRight.Z);
	}

	Vector3 operator- (float fRight) const
	{
		return Vector3(X - fRight, Y - fRight, Z - fRight);
	}

	Vector3 operator* (const Vector3& vecRight) const
	{
		return Vector3(X * vecRight.X, Y * vecRight.Y, Z * vecRight.Z);
	}

	Vector3 operator* (float fRight) const
	{
		return Vector3(X * fRight, Y * fRight, Z * fRight);
	}

	Vector3 operator/ (const Vector3& vecRight) const
	{
		return Vector3(X / vecRight.X, Y / vecRight.Y, Z / vecRight.Z);
	}

	Vector3 operator/ (float fRight) const
	{
		return Vector3(X / fRight, Y / fRight, Z / fRight);
	}

	Vector3 operator - () const
	{
		return Vector3(-X, -Y, -Z);
	}

	void operator += (float fRight)
	{
		X += fRight;
		Y += fRight;
		Z += fRight;
	}

	void operator -= (float fRight)
	{
		X -= fRight;
		Y -= fRight;
		Z -= fRight;
	}

	void operator *= (float fRight)
	{
		X *= fRight;
		Y *= fRight;
		Z *= fRight;
	}

	void operator /= (float fRight)
	{
		X /= fRight;
		Y /= fRight;
		Z /= fRight;
	}
};

struct VehicleColors
{
	BYTE byteColor1;
	BYTE byteColor2;
	BYTE byteColor3;
	BYTE byteColor4;

	VehicleColors()
	{
		byteColor1 = 0;
		byteColor2 = 0;
		byteColor3 = 0;
		byteColor4 = 0;
	}

	VehicleColors(BYTE byteColour1, BYTE byteColour2, BYTE byteColour3, BYTE byteColour4)
	{
		byteColor1 = byteColour1;
		byteColor2 = byteColour2;
		byteColor3 = byteColour3;
		byteColor4 = byteColour4;
	}
};

enum eStateType
{
	STATE_TYPE_DISCONNECT,
	STATE_TYPE_CONNECT,
	STATE_TYPE_SPAWN,
	STATE_TYPE_DEATH,
	STATE_TYPE_ONFOOT,
	STATE_TYPE_ENTERVEHICLE,
	STATE_TYPE_INVEHICLE,
	STATE_TYPE_PASSENGER,
	STATE_TYPE_EXITVEHICLE
};

enum eNetPadKeys
{
	// On Foot/In Vehicle
	NET_PAD_KEY_ENTEREXIT_VEHICLE = 1, // Enter/Exit Vehicle (Key 3)

	// On Foot
	NET_PAD_KEY_SPRINT = 2, // Sprint (Key 1)
	NET_PAD_KEY_JUMP = 4, // Jump (Key 2)
	NET_PAD_KEY_ATTACK = 8, // Attack (Key 4)
	NET_PAD_KEY_FREE_AIM_1 = 16, // Free Aim/Melee Lock On (Key 5)
	NET_PAD_KEY_FREE_AIM_2 = 32, // Free Aim/Melee Lock On 2 (Key 6)
	NET_PAD_KEY_MOUSE_AIM = 64, // Mouse Aim (Key 87)
	NET_PAD_KEY_COMBAT_PUNCH_1 = 128, // Combat Punch 1 (Key 59)
	NET_PAD_KEY_COMBAT_PUNCH_2 = 256, // Combat Punch 2 (Key 60)
	NET_PAD_KEY_COMBAT_KICK = 512, // Combat Kick (Key 62)
	NET_PAD_KEY_COMBAT_BLOCK = 1024, // Combat Block (Key 63)

	// In Vehicle
	NET_PAD_KEY_ACCELERATE = 2048, // Accelerate (Key 40)
	NET_PAD_KEY_REVERSE = 4096, // Reverse (Key 41)
	NET_PAD_KEY_HANDBRAKE_1 = 8192, // Handbrake (Key 44)
	NET_PAD_KEY_HANDBRAKE_2 = 16384, // Handbrake 2 (Key 45)
	NET_PAD_KEY_HORN = 32768, // Horn (Key 54)
	NET_PAD_KEY_DRIVE_BY = 65536, // Drive By (Key 38)
	NET_PAD_KEY_HELI_PRIMARY_FIRE = 131072, // Heli Primary Fire (Key 39)
};


struct NetPadState
{
	BYTE byteLeftAnalogLR[2]; // Left Analog Left/Right (OnFoot: Key 12/13 InCar: Key 30/31)
	BYTE byteLeftAnalogUD[2]; // Left Analog Up/Down (OnFoot: Key 14/15 InVehicle: Key 32/33)
	BYTE byteTriggers[2];
	DWORD dwKeys; // All other keys

	NetPadState()
	{
		memset(this, 0, sizeof(NetPadState));
	}

	NetPadState(BYTE _byteLeftAnalogLR[2], BYTE _byteLeftAnalogUD[2], DWORD _dwKeys)
	{
		byteLeftAnalogLR[0] = _byteLeftAnalogLR[0];
		byteLeftAnalogLR[1] = _byteLeftAnalogLR[1];
		byteLeftAnalogUD[0] = _byteLeftAnalogUD[0];
		byteLeftAnalogUD[1] = _byteLeftAnalogUD[1];
		dwKeys = _dwKeys;
	}

	bool operator== (const NetPadState& o) const
	{
		return (byteLeftAnalogLR[0] == o.byteLeftAnalogLR[0] && 
				byteLeftAnalogLR[1] == o.byteLeftAnalogLR[1] && 
				byteLeftAnalogUD[0] == o.byteLeftAnalogUD[0] && 
				byteLeftAnalogUD[1] == o.byteLeftAnalogUD[1] && 
				dwKeys == o.dwKeys);
	}

	bool operator!= (const NetPadState& o) const
	{
		return (byteLeftAnalogLR[0] != o.byteLeftAnalogLR[0] || 
			byteLeftAnalogLR[1] != o.byteLeftAnalogLR[1] || 
			byteLeftAnalogUD[0] != o.byteLeftAnalogUD[0] || 
			byteLeftAnalogUD[1] != o.byteLeftAnalogUD[1] || 
			dwKeys != o.dwKeys);
	}

	bool IsDoingCombat()
	{
		return (dwKeys & NET_PAD_KEY_COMBAT_PUNCH_1 || dwKeys & NET_PAD_KEY_COMBAT_PUNCH_2 || 
			    dwKeys & NET_PAD_KEY_COMBAT_KICK);
	}

	bool IsFiring()
	{
		return ((dwKeys & NET_PAD_KEY_ATTACK) != 0);
	}

	bool IsAiming()
	{
		return (dwKeys & NET_PAD_KEY_FREE_AIM_1 || dwKeys & NET_PAD_KEY_FREE_AIM_2 || 
				dwKeys & NET_PAD_KEY_MOUSE_AIM);
	}

	bool IsDoingDriveBy()
	{
		return ((dwKeys & NET_PAD_KEY_DRIVE_BY) != 0);
	}

	bool IsFiringHelicoptor()
	{
		return ((dwKeys & NET_PAD_KEY_HELI_PRIMARY_FIRE) != 0);
	}

	bool IsSprinting()
	{
		return ((dwKeys & NET_PAD_KEY_SPRINT) != 0);
	}

	bool IsUsingHorn()
	{
		return ((dwKeys & NET_PAD_KEY_HORN) != 0);
	}

	bool IsJumping()
	{
		return ((dwKeys & NET_PAD_KEY_JUMP) != 0);
	}

	bool IsUsingHandbrake()
	{
		return (dwKeys & NET_PAD_KEY_HANDBRAKE_1 || dwKeys & NET_PAD_KEY_HANDBRAKE_2);
	}
};


#include "CVehicleManagerInterface.h"
#include "CPlayerManagerInterface.h"
#include "CObjectManagerInterface.h"
#include "CPickupManagerInterface.h"
#include "CNetworkManagerInterface.h"
#include "CCheckpointManagerInterface.h"
#include "CBlipManagerInterface.h"
#include "CActorManagerInterface.h"
#include "CTimeInterface.h"
#include "CTrafficLightsInterface.h"
#include "CModuleManagerInterface.h"
#include "CEventsInterface.h"
