--[[
 - Copyright (c) 2011, mabako
 -
 - All rights reserved.
 -
 - Redistribution and use in source and binary forms, with or without modification,
 - are permitted provided that the following conditions are met:
 -
 -     * Redistributions of source code must retain the above copyright notice, this
 -       list of conditions and the following disclaimer.
 -     * Redistributions in binary form must reproduce the above copyright notice, this
 -       list of conditions and the following disclaimer in the documentation and/or other
 -       materials provided with the distribution.
 -     * Neither the name of the mta-modlua nor the names of its contributors may be used
 -       to endorse or promote products derived from this software without specific prior
 -       written permission.
 -
 - THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 - "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 - LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 - A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 - CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 - EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 - PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 - PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 - LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 - NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 - SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--]]
 
local function runcode(code, outputfunc)
	-- try to load the function with return, since it could... return stuff after all
	local func, errormessage = loadstring("return " .. code)
	if errormessage then
		-- load it without return (should work if it is valid)
		func, errormessage = loadstring(code)
		if errormessage then
			-- still failed, output the erorr
			outputfunc(errormessage, false)
			return
		end
	end
	
	-- execute the function
	local results = { pcall(func) }
	if not results[1] then
		-- failed
		outputfunc(results[2], false)
	else
		-- succeeded, collect results
		local str = {}
		for i = 2, #results do
			table.insert( str, tostring(results[i]) .. " [" .. type(results[i]) .. "]" )
		end
		outputfunc("Result: " .. table.concat(str, ", "), true)
	end
end

addEvent("consoleInput",
	function(cmd)
		-- 'lua' command to do runcode stuff
		if cmd:sub(1, 3) == "lua" then
			-- check for params
			if cmd:sub(4, 4) == " " and #cmd > 4 then
				-- execute the code
				runcode(cmd:sub(5),
					function(str, succeeded)
						if succeeded then
							log("[lua] " .. str)
						else
							log("[lua:error] " .. str)
						end
					end
				)
			else
				log("USAGE: lua [code]")
			end
		end
	end
)
