//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CPlayerManagerInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"
#include "CNetworkPlayerInterface.h"

class CPlayerManagerInterface
{
public:
 	virtual bool DoesExist(EntityId playerId) = 0;
	virtual void Add(EntityId playerId, char * sPlayerName) = 0;
	virtual bool Remove(EntityId playerId, BYTE byteReason) = 0;
	virtual bool IsNameInUse(char * szNick) = 0;
	virtual EntityId GetPlayerFromName(char * sNick) = 0;
	virtual EntityId GetPlayerCount() = 0;

	virtual CNetworkPlayerInterface * GetAt(EntityId playerId) = 0;
};