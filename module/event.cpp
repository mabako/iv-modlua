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

#include "event.h"

/**
 * Event constructor - registers itself as an event
 */
event::event(const char* szEventName, vm* v, int iFunction)
{
	active = false;

	// Copy all relevant data
	this->p = v;
	this->iFunction = iFunction;
		
	this->szEventName = new char[strlen(szEventName)+1];
	strcpy(this->szEventName, szEventName);
	this->szEventName[strlen(szEventName)] = 0;

	setActive(true);
}

event::~event( )
{
	setActive(false);

	// free the space for the events name
	delete szEventName;
}

/**
 * Removes itself from the events list
 */
bool event::setActive(bool newActive)
{
	if(newActive == active)
		return false;

	active = newActive;
	if(!newActive)
	{
		// Remove the event
		return InterfaceContainer.g_pEvents->RemoveModuleEvent(szEventName, staticHandler, (void*)this);
	}
	else
	{
		// Register the event
		return InterfaceContainer.g_pEvents->AddModuleEvent(this->szEventName, staticHandler, (void*)this);
	}
}

void event::staticHandler(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn, void* pChunk)
{
	// Call the dynamic handler
	event* e = reinterpret_cast<event*>(pChunk);
	e->handler(pArguments, pReturn);
}

void event::handler(SquirrelArgumentsInterface* pArguments, SquirrelArgumentInterface* pReturn)
{
	if(!active)
		return;

	// Get our state
	lua_State* l = p->getState( );

	// Push the function, then all arguments
	lua_rawgeti(l, LUA_REGISTRYINDEX, iFunction);
	for(unsigned int i = 0; i < pArguments->GetSize( ); ++ i)
		vm::argumentToLua(l, pArguments->Get( i ));

	// Call the function
	if(0 == lua_pcall(l, pArguments->GetSize(), 1, 0))
	{
		// Succeeded, so get the return value (if any)
		vm::luaToArgument(l, -1, pReturn, false);
	}
	else
	{
		// Print the error
		LogPrintf("[lua] %s", lua_tostring(l, -1));
	}

	// Pop the number of return values (third param in pcall)
	lua_pop(l, 1);
}
