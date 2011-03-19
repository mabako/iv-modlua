/*
 * Copyright (c) 2011, mabako
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this
 *       list of conditions and the following disclaimer in the documentation and/or other
 *       materials provided with the distribution.
 *     * Neither the name of the mta-modlua nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without specific prior
 *       written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <map>
#include "Main.h"
#include "SDK/SDK.h"
#include "vm.h"

#if defined(_WIN32)
  #include <windows.h>
#else
  #error("Not supported")
#endif
const char * g_szModuleName = "lua Module";
int base = -0x400000;

/*
 * IV:MP's natives
 */
std::map<std::string, scriptfunction> functions;

/**
 * Script that is initially loaded to... load more scripts
 */
#define scriptloader \
	"log(\"-------== lua ==--------\") " \
	"local config = getConfig() " \
	"if config['lua'] then " \
		"for key, value in pairs(config['lua']) do "\
			"if loadLuaScript(value) then " \
				"log('Loaded lua script ' .. value) " \
			"end " \
		"end " \
	"end " \
	"log(\"          ---\") "

/*
 *	This function is called when the module was loaded.
 *
 *	Return
 *		true: Show a message confirming the module was loaded
 *		false: Don't show a message confirming the module was loaded
 */
EXPORT bool InitModule(char * szModuleName)
{
	strcpy(szModuleName, g_szModuleName);
	LogPrintf("lua Module 1.00 by mabako loaded.");

	// let's dig into the script function pool
	base += (int)GetModuleHandle(0);
	
	// Only works on Windows 0.1 T2 R2 server
	int scriptfuncaddr = (base + 0x514958);
	do
	{
		scriptfunction func = *(scriptfunction*)scriptfuncaddr;
		scriptfuncaddr += sizeof(scriptfunction);
		if( func.sqFunc == 0 )
			break;

		if(strcmp(func.szFunctionName, "addEvent") && strcmp(func.szFunctionName, "callEvent") && strcmp(func.szFunctionName, "removeEvent"))
			functions.insert(std::pair<char*, scriptfunction>(func.szFunctionName, func));
	}
	while( true );

	return true;
}

/*
 *	This function is called every server pulse.
 *	It is not recommended to run CPU-intensive code here.
 */
EXPORT void Pulse()
{
}

/* This function is called when a script is loaded. */
EXPORT void ScriptLoad(HSQUIRRELVM pVM)
{
	static bool bInitalizedScripts = false;
	if(!bInitalizedScripts)
	{
		// Load the script loader - instead of writing it in C/C++, just use lua which has access to native functions already
		vm* v = new vm();
		v->loadString(scriptloader);
		delete v;

		bInitalizedScripts = true;
	}
}

/**
 * SDK function
 */
EXPORT void RegisterSquirrelFunction(const char* szName, SQFUNCTION pFunction)
{
	scriptfunction f;
	f.iParameterCount = -1;
	f.szFunctionTemplate = 0;
	f.sqFunc = pFunction;
	f.szFunctionName = new char[strlen(szName)+1];
	strcpy(f.szFunctionName, szName);
	functions.insert(std::pair<char*, scriptfunction>(f.szFunctionName, f));
}
