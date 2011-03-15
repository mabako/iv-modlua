//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CActorManagerInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"

class CActorManagerInterface
{
public:
	virtual EntityId Create(int iModelId, Vector3 vecPosition, float fHeading) = 0;
	virtual void    Delete(EntityId actorId) = 0;
	virtual void    SetPosition(EntityId actorId, Vector3 vecPosition) = 0;
	virtual void    SetHeading(EntityId actorId, float fHeading) = 0;
	virtual bool    DoesExist(EntityId actorId) = 0;
	virtual EntityId GetActorCount() = 0;
};