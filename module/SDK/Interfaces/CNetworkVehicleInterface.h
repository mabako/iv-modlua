//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CNetworkVehicleInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"

class CNetworkVehicleInterface
{
public:
	virtual void Reset() = 0;
	virtual void Respawn() = 0;
	virtual EntityId GetDriverId() = 0;
	virtual EntityId GetPassengerId(BYTE byteSeatId) = 0;
	virtual void SetModel(int iModelId) = 0;
	virtual int GetModel() = 0;
	virtual void SetHealth(unsigned int uHealth) = 0;
	virtual unsigned int GetHealth() = 0;
	virtual void SetEngineHealth(unsigned int uEngineHealth) = 0;
	virtual unsigned int GetEngineHealth() = 0;
	virtual void SetPosition(Vector3 vecPosition) = 0;
	virtual void GetPosition(Vector3 * vecPosition) = 0;
	virtual void SetRotation(Vector3 vecRotation) = 0;
	virtual void GetRotation(Vector3 * vecRotation) = 0;
	virtual void SetDirtLevel(float fDirtLevel) = 0;
	virtual float GetDirtLevel() = 0;
	virtual void SetTurnSpeed(Vector3 vecTurnSpeed) = 0;
	virtual void GetTurnSpeed(Vector3 * vecTurnSpeed) = 0;
	virtual void SetMoveSpeed(Vector3 vecMoveSpeed) = 0;
	virtual void GetMoveSpeed(Vector3 * vecMoveSpeed) = 0;
	virtual void SetColor(VehicleColors colors) = 0;
	virtual void GetColor(VehicleColors * colors) = 0;
	virtual void SoundHorn(unsigned int iDuration) = 0;
	virtual void SetSirenState(bool bSirenState) = 0;
	virtual bool GetSirenState() = 0;
	virtual void SetLocked(unsigned char ucLocked) = 0;
	virtual unsigned char GetLocked() = 0;
	virtual void SetIndicatorState(bool bFrontLeft, bool bFrontRight, bool bBackLeft, bool bBackRight) = 0;
	virtual bool GetIndicatorState(unsigned char ucSlot) = 0;
	virtual void SetComponentState(unsigned char ucSlot, bool bOn) = 0;
	virtual bool GetComponentState(unsigned char ucSlot) = 0;
	virtual void ResetComponents(bool bNotify = true) = 0;
	virtual void SetVariation(unsigned char ucVariation) = 0;
	virtual unsigned char GetVariation() = 0;
};