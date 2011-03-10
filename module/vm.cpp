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

#include <vector>
#include "vm.h"
#include "Main.h"

/**
 * Initalizes a new lua vm
 */
vm::vm()
{
	l = luaL_newstate();

	// Register libraries
	luaL_openlibs(l);

	// Initalize our own functions
	init();
}

/**
 * Close the state
 */
vm::~vm()
{
	lua_close(l);
	l = 0;
}

/**
 * Loads a script file
 * @return true if successful, false otherwise
 */
bool vm::loadScript(const char* name)
{
	return LUA_OK == luaL_dofile(l, name);
}

/**
 * Loads a string
 * @return true if successful, false otherwise
 */
bool vm::loadString(const char* string)
{
	return LUA_OK == luaL_dostring(l, string);
}

/**
 * Lua function handler
 */
int functionhandler(lua_State* l)
{
	LogPrintf("Called Function");
	return 0;
}

/**
 * Initalize squirrel functions
 */
void vm::init()
{
	for(std::vector<scriptfunction>::iterator iter = functions.begin(); iter != functions.end(); ++ iter)
	{
		lua_register(l, (*iter).szFunctionName, &functionhandler);
	}
}
