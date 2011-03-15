//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CModuleManagerInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"
#include "CModuleInterface.h"

class CModuleManagerInterface
{
public:
	virtual CModuleInterface * LoadModule(const char * szName) = 0;
};