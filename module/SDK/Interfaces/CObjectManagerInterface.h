//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CObjectManagerInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"

class CObjectManagerInterface
{
public:
	virtual EntityId Create(DWORD dwModelHash, float fX, float fY, float fZ, float fRX, float fRY, float fRZ) = 0;
	virtual void Delete(EntityId objectId) = 0;
	virtual bool DoesExist(EntityId objectId) = 0;

	virtual EntityId GetObjectCount() = 0;

	virtual DWORD GetModel(EntityId objectId) = 0;
	virtual bool SetPosition(EntityId objectId, Vector3 vecPosition) = 0;
	virtual bool GetPosition(EntityId objectId, Vector3* vecPosition) = 0;
	virtual bool SetRotation(EntityId objectId, Vector3 vecRotation) = 0;
	virtual bool GetRotation(EntityId objectId, Vector3* vecRotation) = 0;
};