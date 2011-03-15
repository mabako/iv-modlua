//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CScriptingManagerInterface.h
//======================================================================

#pragma once
#include "InterfaceCommon.h"
#include "CSquirrelInterface.h"

class CScriptingManagerInterface
{
public:
	virtual void RegisterConstant(const char * szConstantName, bool bValue) = 0;
	virtual void RegisterConstant(const char * szConstantName, int iValue) = 0;
	virtual void RegisterConstant(const char * szConstantName, float fValue) = 0;
	virtual void RegisterConstant(const char * szConstantName, const char * szValue) = 0;
};