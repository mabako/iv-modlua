//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CBlipManagerInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"

class CBlipManagerInterface
{
public:
	virtual EntityId     Create(int iSprite, Vector3 vecPosition) = 0;
	virtual void         Delete(EntityId blipId) = 0;
	virtual void         SetPosition(EntityId blipId, Vector3 vecPosition) = 0;
	virtual Vector3      GetPosition(EntityId blipId) = 0;
	virtual void         SetColor(EntityId blipId, unsigned int color) = 0;
	virtual unsigned int GetColor(EntityId blipId) = 0;
	virtual void         SetSize(EntityId blipId, float size) = 0;
	virtual float        GetSize(EntityId blipId) = 0;
	virtual void	     ToggleShortRange(EntityId blipId, bool bShortRange) = 0;
	virtual void	     ToggleRoute(EntityId blipId, bool bRoute) = 0;
	virtual void         HandleClientJoin(EntityId playerId) = 0;
	virtual bool         DoesExist(EntityId blipId) = 0;
	virtual EntityId     GetBlipCount() = 0;
};