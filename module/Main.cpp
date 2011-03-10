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
	
	// Only works on Windows 0.1 T1 R2 server
	int scriptfuncaddr = (base + 0x526A40);
	do
	{
		scriptfunction func = *(scriptfunction*)scriptfuncaddr;
		scriptfuncaddr += sizeof(scriptfunction);
		if( func.sqFunc == 0 )
			break;

		functions.insert(std::pair<char*, scriptfunction>(func.szFunctionName, func));
	}
	while( true );

	LogPrintf("OOO---------------");
	vm* v = new vm();
	v->loadString("print(\"Hello World\") local a = kickPlayer(\"asdf\"); print(tostring(a) .. \"=\" .. type(a))");
	delete v;
	LogPrintf("OOO---------------");

	return true;
}

/* This function is called when a script is loaded. */
EXPORT void ScriptLoad(HSQUIRRELVM pVM)
{
}

/* This function is called when a script is unloaded. */
EXPORT void ScriptUnload(HSQUIRRELVM pVM)
{
}

/*
 *	This function is called for internal server callbacks.
 *	Usage reserved.
 *
 *	Return
 *		true: Callback was handled.
 *		false: Callback was not handled.
 */
EXPORT bool HandleCallback(eModuleCallback Callback, void *pArgs)
{
	return false;
}

/*
 *	This function is called every server pulse.
 *	It is not recommended to run CPU-intensive code here.
 */
EXPORT void Pulse()
{	
}
