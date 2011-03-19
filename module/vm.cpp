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
#include "event.h"

/**
 * VM list
 */
std::list<vm*> vm::vms;

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

	// add it to our list of VMs
	vms.push_back(this);

	// create the state
	l = luaL_newstate();

	// Register libraries
	luaL_openlibs(l);

	// Initalize our own functions
	init();

	// Create the event table
	lua_pushnumber(l, 1);
	lua_newtable(l);
	lua_settable(l, LUA_REGISTRYINDEX);
}

/**
 * Close the state
 */
vm::~vm()
{
	lua_close(l);
	l = 0;

	// Remove it from our list of VMs
	vms.remove(this);
}

/**
 * Finds the VM for that lua state
 */
vm* vm::getVM(lua_State* l)
{
	for(std::list< vm* >::const_iterator iter = vms.begin(); iter != vms.end(); ++ iter)
		if((*iter)->getState() == l)
			return *iter;
	return 0;
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
void vm::luaToSquirrel(lua_State* l, int i)
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
void vm::squirrelToLua(lua_State* l, int sqtop)
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
					squirrelToLua(l, -2);
					squirrelToLua(l, -1);
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
 * Pushes any lua arguments to the squirrel argument
 */
void vm::luaToArgument(lua_State* l, int i, SquirrelArgumentInterface* pArgument, bool changeIfNil)
{
	// Guess type based on the lua type
	switch(lua_type(l, i))
	{
		case LUA_TNIL:
			if(changeIfNil)
				pArgument->SetNull( );
			break;
		case LUA_TBOOLEAN:
			pArgument->SetBool(lua_toboolean(l, i) != 0);
			break;
		case LUA_TNUMBER:
			{
				// no different type for integers and floats
				float f = lua_tonumber(l, i);
				int i = (int)f;
				if(abs(f-i) < 0.000001)
					pArgument->SetInteger(i);
				else
					pArgument->SetFloat(f);
			}
			break;
		case LUA_TSTRING:
			pArgument->SetString(lua_tostring(l, i));
			break;
		default:
			LogPrintf("[lua:%s] Unknown type %d", lua_tostring(l, 1), lua_type(l, i));
			break;
	}
}

/**
 * Pushes any argument to the lua VM
 */
void vm::argumentToLua(lua_State* l, SquirrelArgumentInterface* pArgument)
{
	if(!pArgument)
	{
		lua_pushnil(l);
		return;
	}

	switch(pArgument->GetType( ))
	{
		case OT_NULL:
			lua_pushnil(l);
			break;
		case OT_INTEGER:
			lua_pushinteger(l, pArgument->GetInteger( ));
			break;
		case OT_STRING:
			lua_pushstring(l, pArgument->GetString( ));
			break;
		case OT_BOOL:
			lua_pushboolean(l, pArgument->GetBool( ));
			break;
		case OT_FLOAT:
			lua_pushnumber(l, pArgument->GetFloat( ));
			break;
		case OT_TABLE:
			{
				lua_newtable(l);

				SquirrelArgumentsInterface* pTemp = pArgument->GetTable( );
				for(unsigned int i = 0; i < pTemp->GetSize( ); i += 2)
				{
					argumentToLua(l, pTemp->Get(i));
					argumentToLua(l, pTemp->Get(i+1));
					lua_settable(l, -3);
				}
			}
			break;
		case OT_ARRAY:
			{
				lua_newtable(l);

				SquirrelArgumentsInterface* pTemp = pArgument->GetTable( );
				for(unsigned int i = 0; i < pTemp->GetSize( ); i ++)
				{
					lua_pushinteger(l, i);
					argumentToLua(l, pTemp->Get(i));
					lua_settable(l, -3);
				}
			}
			break;
		default:
			lua_pushnil(l);
			break;
	}
}

/**
 * Gets the lua state
 */
lua_State* vm::getState()
{
	return l;
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
					luaToSquirrel(l, i);
			}

			// Save the stack top
			int sqtop = sq_gettop(sq);

			// Call the function
			(*iter).second.sqFunc(sq);

			// Pass all return values to the lua vm
			int iRet = 0;
			for( ++sqtop; sqtop <= sq_gettop(sq); ++ sqtop)
			{
				squirrelToLua(l, sqtop);
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
 * This is, coincidentally, similar to MTA's
 */
int vm::luaToRef(lua_State* l, int i)
{
	// Save the stack top
	int top = lua_gettop(l);

	// Create a name for it
	char buff[10] = {0};
	char* index = _itoa((int)lua_topointer(l, i), buff, 16);

	// Get the events table
	int ref = 0;
	lua_rawgeti(l, LUA_REGISTRYINDEX, 1);
	lua_getfield(l, -1, index);
	ref = (int)lua_tonumber(l, -1);
	lua_pop(l, 2);

	// See if it is a reference yet
	if(ref == 0)
	{
		// Add a new reference
		lua_settop(l, i);
		ref = luaL_ref(l, LUA_REGISTRYINDEX);

		// Add it to the table
		lua_rawgeti(l, LUA_REGISTRYINDEX, 1);
		lua_pushstring(l, index);
		lua_pushnumber(l, ref);
		lua_settable(l, -3);
		lua_pop(l, 1);
	}

	// Restore the stack
	lua_settop(l, top);
	return ref;
}

/**
 * Adds an event
 * lua syntax: bool addEvent(name, function)
 */
int vm::addEvent(lua_State* l)
{
	if(lua_type(l, 1) == LUA_TSTRING && lua_type(l, 2) == LUA_TFUNCTION)
	{
		vm* vm = getVM(l);
		if(vm)
		{
			const char* szEvent = lua_tostring(l, 1);
			int iFunction = luaToRef(l, 2);

			// check if event exists with the same handler
			for(std::list< event* >::const_iterator iter = vm->events.begin(); iter != vm->events.end(); ++ iter)
			{
				if((*iter)->iFunction == iFunction && !strcmp((*iter)->szEventName, szEvent))
				{
					if((*iter)->active == true)
						lua_pushboolean(l, false);
					else
					{
						(*iter)->setActive(true);
						vm->events.push_back(*iter);
						lua_pushboolean(l, true);
					}
					return 1;
				}
			}
			
			vm->events.push_back(new event(szEvent, vm, iFunction));
			lua_pushboolean(l, true);
			return 1;
		}
	}

	lua_pushboolean(l, false);
	return 1;
}

/**
 * Calls an event
 * lua syntax: retval callEvent(name, [default return = 1, [...])
 */
int vm::callEvent(lua_State *l)
{
	if(lua_type(l, 1) == LUA_TSTRING)
	{
		const char* eventName = lua_tostring(l, 1);

		SquirrelArgumentInterface* pReturn = InterfaceContainer.pSquirrelArgumentManager->CreateArgument( );
		SquirrelArgumentsInterface* pArguments = InterfaceContainer.pSquirrelArgumentManager->CreateArguments( );
		if(lua_type(l, 2) != LUA_TNONE)
		{
			luaToArgument(l, 2, pReturn);

			int i = 3;
			while(lua_type(l, i) != LUA_TNONE)
			{
				SquirrelArgumentInterface* p = pArguments->Add( );
				luaToArgument(l, i, p);
				++ i;
			}
		}

		InterfaceContainer.g_pEvents->CallModuleEvent(eventName, pArguments, pReturn);
		argumentToLua(l, pReturn);

		InterfaceContainer.pSquirrelArgumentManager->DeleteArguments(pArguments);
		InterfaceContainer.pSquirrelArgumentManager->DeleteArgument(pReturn);

		return 1;
	}
	lua_pushboolean(l, false);
	return 1;
}

/**
 * Removes an event
 * lua syntax: bool removeEvent(name, [function])
 */
int vm::removeEvent(lua_State* l)
{
	if(lua_type(l, 1) == LUA_TSTRING)
	{
		const char* szEvent = lua_tostring(l, 1);
		if(lua_type(l, 2) == LUA_TFUNCTION || lua_type(l, 2) == LUA_TNONE)
		{
			bool found = false;

			// find the VM
			vm* vm = getVM(l);
			if(vm)
			{
				for(std::list< event* >::iterator iter = vm->events.begin(); iter != vm->events.end(); )
				{
					if(!strcmp((*iter)->szEventName, szEvent))
					{
						if(lua_type(l, 2) == LUA_TNONE || (*iter)->iFunction == luaToRef(l, 2))
						{
							if((*iter)->active == true)
							{
								(*iter)->setActive(false);
								vm->events.erase(iter++);
								found = true;
							}
						}
						else
							iter++;
					}
				}
			}

			lua_pushboolean(l, found);
			return 1;
		}
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
	lua_register(l, "addEvent", &addEvent);
	lua_register(l, "callEvent", &callEvent);
	lua_register(l, "removeEvent", &removeEvent);

	// redirects for all functions
	char szRedirect[256] = {0};
	for(std::map<std::string, scriptfunction>::iterator iter = functions.begin(); iter != functions.end(); ++ iter)
	{
		sprintf_s(szRedirect, sizeof(szRedirect), "function %s(...) return sqInvoke(\"%s\", ...) end", (*iter).first.c_str(), (*iter).first.c_str());
		loadString(szRedirect);
	}
}
