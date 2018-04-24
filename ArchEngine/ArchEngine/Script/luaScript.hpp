/*===========================================================================*
 * Arch Engine - "Script/luaScript.hpp                                       *
 *                                                                           *
 * Class responsible for communication between the engine and Lua Scripts.   *
 * Based in:                                                                 *
 * - (https://eliasdaler.wordpress.com/2013/10/11/lua_cpp_binder/)           *
 * - Roberto Ierusalimschy's Programming in Lua, 4th edition                 *
 * - Game Programming Gems 6                                                 *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 15/04/2018                                                       *
 * Last Modified: 19/04/2018                                                 *
 *===========================================================================*/


#ifndef SCRIPT_LUA_SCRIPT_HPP
#define SCRIPT_LUA_SCRIPT_HPP


#include "../Core/engineMacros.hpp"
#include "../Utils/serviceLocator.hpp"

#include <lua.hpp>

#include <cassert>
#include <cstdarg>
#include <string>
#include <utility>
#include <vector>


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
		void initialize(const std::string& path);
		void destroy();

		void clearStack();

		// Binders
		template<typename T>
		T get(const std::string& var_name);

		// Return vectors are moved, not copied
		std::vector<int> getIntVector(const std::string& name);
		std::vector<std::string> getTableKeys(const std::string& name);
		std::vector<std::pair<std::string, std::string>>
			getTablePairs(const std::string& name);

		bool getToStack(const std::string& var_name, int& level);

		template<typename T>
		T luaGet(const std::string& var_name) {
			return 0;
		}

		template<typename T>
		T luaGetDefault() {
			return 0;
		}

		// Error handling
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		void printError(const std::string& var_name, const std::string& motive);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		void printStack();
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

	private:
		enum LuaScriptState {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		// Flag to ensure resources have been freed
		LuaScriptState m_state;

		// Lua internal state
		lua_State* m_lua;

		// Script path
		std::string m_path;
	};


	//------------------------------------------------ Template implementations
	template<typename T>
	T LuaScript::get(const std::string& var_name) {
		if (!m_lua) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			printError(var_name, "Script is not loaded");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return luaGetDefault<T>();
		}

		T result;

		// Used to store the top level of the stack
		int level;

		if (getFromStack(var_name, level))
			result = luaGet<T>(var_name);
		else
			result = luaGetDefault<T>();

		lua_pop(m_lua, level + 1);
		return result;
	}

	//--- "get methods" specializations ---//
	// inlining to avoid linker errors

	// Bool
	template <>
	inline bool LuaScript::luaGet<bool>(const std::string& var_name) {
		return (bool)lua_toboolean(m_lua, -1);
	}

	// Float
	template <>
	inline float LuaScript::luaGet<float>(const std::string& var_name) {
		if (!lua_isnumber(m_lua, -1))
			printError(var_name, "Not a number");

		return (float)lua_tonumber(m_lua, -1);
	}

	// Int
	template <>
	inline int LuaScript::luaGet<int>(const std::string& var_name) {
		if (!lua_isnumber(m_lua, -1))
			printError(var_name, "Not a number");

		return (int)lua_tonumber(m_lua, -1);
	}

	// String
	template <>
	inline std::string LuaScript::luaGet<std::string>(const std::string& var_name) {
		std::string s = "";
		if (lua_isstring(m_lua, -1))
			s = std::string(lua_tostring(m_lua, -1));
		else
			printError(var_name, "Not a string");

		return s;
	}

	template<>
	inline std::string LuaScript::luaGetDefault<std::string>() {
		return "";
	}
}


#endif	// SCRIPT_LUA_SCRIPT_HPP
