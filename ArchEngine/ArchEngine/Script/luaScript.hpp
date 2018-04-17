/*===========================================================================*
 * Arch Engine - "Script/luaScript.hpp                                       *
 *                                                                           *
 * Class responsible for communication between the engine and Lua Scripts.   *
 * Based in:                                                                 *
 * - Roberto Ierusalimschy's Programming in Lua, 4th edition                 *
 * - Game Programming Gems 6                                                 *
 * - (https://eliasdaler.wordpress.com/2013/10/11/lua_cpp_binder/)           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 15/04/2018                                                       *
 * Last Modified: 16/04/2018                                                 *
 *===========================================================================*/


#ifndef SCRIPT_LUA_SCRIPT_HPP
#define SCRIPT_LUA_SCRIPT_HPP


#include "../Core/engineMacros.hpp"
#include "../Utils/serviceLocator.hpp"

#include <lua.hpp>

#include <cassert>
#include <cstdarg>
#include <string>


namespace Script {
	class LuaScript {
	public:
		LuaScript();
		~LuaScript();

		// Memory related stuff is initialized and destroyed mannualy (and not
		// on the constructor/destructor), due to the fact that if we have
		// multiple script instances inside a container ~LuaScript() might
		// be called, invalidating its state, which is difficult to see.
		// The class has a flag (m_state) to ensure, through assertion, that
		// the user will remember to call the methods.
		void initialize(lua_State* lua, const std::string& path);
		void destroy();

		void clearStack(lua_State* lua);

		// Binders
		template<typename T>
		T get(lua_State* lua, const std::string& var_name);
		
		bool getFromStack(lua_State* lua,
			const std::string& var_name, int& level);

		template<typename T>
		T luaGet(lua_State* lua, const std::string& var_name);

		template<typename T>
		T luaGetDefault();

		// Error handling
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		void printError(const std::string& var_name, const std::string& motive);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		void printStack(lua_State* lua);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

	private:
		enum LuaScriptState {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		// Flag to ensure resources have been freed
		LuaScriptState m_state;

		// Script path
		std::string m_path;
	};


	//------------------------------------------------ Template implementations
	template<typename T>
	T LuaScript::get(lua_State* lua, const std::string& var_name) {
		if (!lua) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			printError(var_name, "Script is not loaded");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return luaGetDefault<T>();
		}

		T result;

		// Used to store the top level of the stack
		int level;

		if (getFromStack(var_name, &level))
			result = luaGet<T>()
		else
			result = luaGetDefault<T>();

		lua_pop(lua, stack_top + 1);
		return result;
	}

	bool LuaScript::getFromStack(lua_State* lua,
		const std::string& var_name, int& level) {
		level = 0;

		std::string var = "";

		for (unsigned i = 0; i < var_name.size(); i++) {
			if (var_name.at(i) == '.') {
				if (level == 0)
					lua_getglobal(lua, var.c_str());
				else
					lua_getfield(lua, -1, var.c_str());

				if (lua_isnil(lua, -1)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
					printError(var_name, var + " is not defined");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
					return false;
				}
				else {
					var = "";
					level++;
				}
			}
			else var += var_name.at(i);
		}

		if (level == 0)
			lua_getglobal(lua, var.c_str());
		else
			lua_getfield(lua, -1, var.c_str());

		if (lua_isnil(lua, -1)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			printError(var_name, var + " is not defined");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		return true;
	}

	//--- "get methods" specializations ---//

	// Returns 0 if int, float or double
	template<typename T>
	T luaGet(lua_State* lua, const std::string& var_name) {
		return 0;
	}

	template<typename T>
	T luaGetDefault(lua_State* lua, const std::string& var_name) {
		return 0;
	}

	// Bool
	template <>
	bool LuaScript::luaGet<bool>(lua_State* lua,
		const std::string& var_name) {
		return (bool)lua_toboolean(lua, -1);
	}

	// Float
	template <>
	float LuaScript::luaGet<float>(lua_State* lua,
		const std::string& var_name) {
		if (!lua_isnumber(lua, -1)) {
			printError(var_name, "Not a number");
		}
		return (float)lua_tonumber(lua, -1);
	}

	// Int
	template <>
	int LuaScript::luaGet<int>(lua_State* lua,
		const std::string& var_name) {
		if (!lua_isnumber(lua, -1)) {
			printError(var_name, "Not a number");
		}
		return (int)lua_tonumber(lua, -1);
	}

	// String
	template <>
	std::string LuaScript::luaGet<std::string>(lua_State* lua,
		const std::string& var_name) {
		std::string s = "null";
		if (lua_isstring(lua, -1)) {
			s = std::string(lua_tostring(lua, -1));
		}
		else {
			printError(var_name, "Not a string");
		}
		return s;
	}

	template<>
	std::string LuaScript::luaGetDefault<std::string>() {
		return "";
	}
}


#endif	// SCRIPT_LUA_SCRIPT_HPP
