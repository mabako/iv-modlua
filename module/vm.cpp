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
#include "vm.h"
#include "Main.h"

/**
 * Squirrel VM
 */
SQVM * vm::sq = 0;

/**
 * Initalizes a new lua vm
 */
vm::vm()
{
	// Create the squirrel vm if it doesn't exist yet
	// TODO: kill this some time.
	if(!sq)
		sq = sq_open(1024);

	// create the state
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
	std::string n("lua/");
	n.append(name);
	if(LUA_OK == luaL_loadfile(l, n.c_str()))
		if(LUA_OK == lua_pcall(l, 0, LUA_MULTRET, 0))
			return true;

	// Output the lua error
	LogPrintf("[lua] %s", lua_tostring(l, -1));

	return false;
}

/**
 * Loads a string
 * @return true if successful, false otherwise
 */
bool vm::loadString(const char* string)
{
	if(LUA_OK == luaL_loadstring(l, string))
		if(LUA_OK == lua_pcall(l, 0, LUA_MULTRET, 0))
			return true;

	// Output the lua error
	LogPrintf("[lua] %s", lua_tostring(l, -1));

	return false;
}

/**
 * push any lua types to squirrel
 * warning: might be inaccurate for floats & ints
 */
void vm::sq_pushAny(lua_State* l, int i)
{
	// Guess type based on the lua type
	switch(lua_type(l, i))
	{
		case LUA_TNIL:
			sq_pushnull(sq);
			break;
		case LUA_TBOOLEAN:
			sq_pushbool(sq, lua_toboolean(l, i));
			break;
		case LUA_TNUMBER:
			{
				// no different type for integers and floats
				float f = lua_tonumber(l, i);
				int i = (int)f;
				if(abs(f-i) < 0.000001)
					sq_pushinteger(sq, i);
				else
					sq_pushfloat(sq, f);
			}
			break;
		case LUA_TSTRING:
			sq_pushstring(sq, lua_tostring(l, i), -1);
			break;
		default:
			LogPrintf("[lua:%s] Unknown type %d", lua_tostring(l, 1), lua_type(l, i));
			break;
	}
}

/**
 * push any squirrel types to lua
 */
void vm::lua_pushAny(lua_State* l, int sqtop)
{
	switch(sq_gettype(sq, sqtop))
	{
		case OT_BOOL:
			{
				SQBool b;
				sq_getbool(sq, sqtop, &b);
				lua_pushboolean(l, b != 0);
			}
			break;
		case OT_INTEGER:
			{
				int i;
				sq_getinteger(sq, sqtop, &i);
				lua_pushinteger(l, i);
			}
			break;
		case OT_FLOAT:
			{
				float f;
				sq_getfloat(sq, sqtop, &f);
				lua_pushnumber(l, f);
			}
			break;
		case OT_STRING:
			{
				const char* c;
				sq_getstring(sq, sqtop, &c);
				lua_pushstring(l, c);
			}
			break;
		case OT_ARRAY:
		case OT_TABLE:
			{
				lua_newtable(l);

				sq_push(sq, sqtop);
				sq_pushnull(sq);

				while(SQ_SUCCEEDED(sq_next(sq, -2)))
				{
					// value = -1, key = -2
					lua_pushAny(l, -2);
					lua_pushAny(l, -1);
					lua_settable(l, -3);

					sq_pop(sq, 2);
				}

				sq_pop(sq, 2);
			}
			break;
		default:
			lua_pushnil(l);
			break;
	}
}

/**
 * Lua function handler
 */
int vm::sqInvoke(lua_State* l)
{
	if(lua_type(l, 1) == LUA_TSTRING)
	{
		// Find the function
		std::map<std::string, scriptfunction>::iterator iter = functions.find(lua_tostring(l, 1));
		if(iter != functions.end())
		{
			// Reset the squirrel vm
			sq_settop(sq, 0);
			sq_pushroottable(sq);

			// push all arguments
			unsigned int i = 1;
			while(lua_type(l, ++ i) != LUA_TNONE)
			{
				// Check type mask
				bool bFoundType = false;
				if((*iter).second.szFunctionTemplate != 0 && strlen((*iter).second.szFunctionTemplate) > (i-2))
				{
					bFoundType = true;
					switch((*iter).second.szFunctionTemplate[i-2])
					{
						case 'b':
							sq_pushbool(sq, lua_toboolean(l, i));
							break;
						case 'f':
							sq_pushfloat(sq, lua_tonumber(l, i));
							break;
						case 's':
							sq_pushstring(sq, lua_tostring(l, i), -1);
							break;
						case 'i':
							sq_pushinteger(sq, lua_tointeger(l, i));
							break;
						default:
							LogPrintf("[lua:%s] Unknown typemask '%s' ('%c')", lua_tostring(l, 1), (*iter).second.szFunctionTemplate, (*iter).second.szFunctionTemplate[i-2]);
							bFoundType = false;
							break;
					}
				}

				if(!bFoundType)
					// Guess the type to push
					sq_pushAny(l, i);
			}

			// Save the stack top
			int sqtop = sq_gettop(sq);

			// Call the function
			(*iter).second.sqFunc(sq);

			// Pass all return values to the lua vm
			int iRet = 0;
			for( ++sqtop; sqtop <= sq_gettop(sq); ++ sqtop)
			{
				lua_pushAny(l, sqtop);
				++ iRet;
			}
			return iRet;
		}
		else
		{
			LogPrintf("[lua] Tried to invoke unknown function %s", lua_tostring(l, 1));
		}
	}
	return 0;
}

/**
 * Loads a script in a new VM
 * lua syntax: bool loadLuaScript(name)
 */
int vm::loadLuaScript(lua_State* l)
{
	if(lua_type(l, 1) == LUA_TSTRING)
	{
		vm* v = new vm();
		bool success = v->loadScript(lua_tostring(l, 1));
		if(!success)
			delete v;

		lua_pushboolean(l, success);
		return 1;
	}

	lua_pushboolean(l, false);
	return 1;
}

/**
 * Initalize squirrel functions
 */
void vm::init()
{
	// register the invoke function
	lua_register(l, "sqInvoke", &sqInvoke);
	lua_register(l, "loadLuaScript", &loadLuaScript);

	// redirects for all functions
	char szRedirect[256] = {0};
	for(std::map<std::string, scriptfunction>::iterator iter = functions.begin(); iter != functions.end(); ++ iter)
	{
		sprintf_s(szRedirect, sizeof(szRedirect), "function %s(...) return sqInvoke(\"%s\", ...) end", (*iter).first.c_str(), (*iter).first.c_str());
		loadString(szRedirect);
	}
}
