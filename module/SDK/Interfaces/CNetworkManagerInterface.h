//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CNetworkManagerInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"

class CNetworkManagerInterface
{
public:
	virtual bool AddBan(char * szIP, unsigned int uiSeconds) = 0;
};