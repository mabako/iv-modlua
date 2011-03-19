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

#pragma once

#include <list>
#include <vector>
#include "SDK/SDK.h"
#include <lua.hpp>

class event;

class vm
{
	friend class event;

private:
	static std::list<vm*> vms;
	static SQVM * sq;

	std::list<event*> events;
	lua_State* l;
public:
	vm();
	~vm();

	static vm* getVM(lua_State* l);

	bool loadScript(const char* name);
	bool loadString(const char* string);

private:
	lua_State* getState();
	static int sqInvoke(lua_State* l);
	static void luaToSquirrel(lua_State* l, int i);
	static void squirrelToLua(lua_State* l, int sqtop);
	static void luaToArgument(lua_State* l, int i, SquirrelArgumentInterface* pArgument, bool changeIfNil = true);
	static void argumentToLua(lua_State* l, SquirrelArgumentInterface* pArgument);
	static int luaToRef(lua_State* l, int i);

	static int loadLuaScript(lua_State* l);

	static int addEvent(lua_State* l);
	static int callEvent(lua_State* l);
	static int removeEvent(lua_State* l);

	void init();
};
