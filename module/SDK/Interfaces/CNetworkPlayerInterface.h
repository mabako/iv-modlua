//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CNetworkPlayerInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"

class CNetworkPlayerInterface
{
public:
	virtual eStateType    GetState() = 0;
	virtual void          SpawnForPlayer(EntityId playerId) = 0;
	virtual void          KillForPlayer(EntityId playerId) = 0;
	virtual void          SpawnForWorld() = 0;
	virtual void          KillForWorld() = 0;
	virtual bool          SetName(char * sName, bool requestedByClient = false) = 0;
	virtual const char*   GetNameConst() = 0;
	virtual bool          IsSpawned() = 0;
	virtual bool          SetModel(int iModelId) = 0;
	virtual int           GetModel() = 0;
	virtual EntityId      GetVehicleId() = 0;
	virtual bool          IsInAnyVehicle() = 0;
	virtual BYTE          GetSeatId() = 0;
	virtual void          SetPosition(Vector3 vecPosition) = 0;
	virtual void          GetPosition(Vector3 * vecPosition) = 0;
	virtual void          SetHeading(float fHeading) = 0;
	virtual float         GetHeading() = 0;
	virtual void          SetMoveSpeed(Vector3 vecMoveSpeed) = 0;
	virtual void          GetMoveSpeed(Vector3 * vecMoveSpeed) = 0;
	virtual void          SetDuckState(bool bDuckState) = 0;
	virtual bool          GetDuckState() = 0;
	virtual void          SetHealth(unsigned int uHealth) = 0;
	virtual unsigned int  GetHealth() = 0;
	virtual void          SetArmour(unsigned int uArmour) = 0;
	virtual unsigned int  GetArmour() = 0;
	virtual void          SetSpawnLocation(Vector3 vecPosition, float fHeading) = 0;
	virtual void          GetSpawnLocation(Vector3 * vecPosition, float * fHeading) = 0;
	virtual bool          GiveMoney(int iMoney) = 0;
	virtual bool          SetMoney(int iMoney) = 0;
	virtual int           GetMoney() = 0;
	virtual void          SetWeapon(unsigned int uWeapon) = 0;
	virtual unsigned int  GetWeapon() = 0;
	virtual void          SetAmmo(unsigned int uAmmo) = 0;
	virtual unsigned int  GetAmmo() = 0;
	virtual const char *  GetSerialConst() = 0;
	virtual void          SetPadState(NetPadState * padState) = 0;
	virtual void          GetPreviousPadState(NetPadState * padState) = 0;
	virtual void          GetPadState(NetPadState * padState) = 0;
	virtual void          SetColor(unsigned int color) = 0;	
	virtual unsigned int  GetColor() = 0;
	virtual unsigned short GetPing() = 0;
	virtual void          ResetClothes() = 0;
	virtual void          SetClothes(unsigned char ucBodyPart, unsigned char ucClothes) = 0;
	virtual unsigned char GetClothes(unsigned char ucBodyPart) = 0;
};