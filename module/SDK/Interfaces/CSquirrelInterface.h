//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CSquirrelInterface.h
//======================================================================

#pragma once
#include "InterfaceCommon.h"

class CSquirrelInterface
{
public:
	virtual SQVM * GetVM() = 0;
};