//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CCheckpointManagerInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"

class CCheckpointManagerInterface
{
public:
	virtual EntityId Add(WORD wType, Vector3 vecPosition, Vector3 vecTargetPosition, float fRadius) = 0;
	virtual void Delete(EntityId checkpointId) = 0;
	virtual bool DoesExist(EntityId checkpointId) = 0;
	virtual EntityId GetCheckpointCount() = 0;
};