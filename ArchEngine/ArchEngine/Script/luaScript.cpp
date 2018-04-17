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


#include "luaScript.hpp"


using namespace Utils;


namespace Script {
	LuaScript::LuaScript() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"LuaScript constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	}

	LuaScript::~LuaScript() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == CONSTRUCTED || m_state == SAFE_TO_DESTROY);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"LuaScript destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	}

	void LuaScript::initialize(lua_State* lua, const std::string& path) {
		m_path = path;

		if (luaL_loadfile(lua, path.c_str()) || lua_pcall(lua, 0, 0, 0)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Failed to load(" + path + ")");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		}
		else
			m_state = INITIALIZED;
	}

	void LuaScript::destroy() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		m_state = SAFE_TO_DESTROY;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			m_path + " window destroyed");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	}

	void LuaScript::clearStack(lua_State* lua) {
		lua_pop(lua, lua_gettop(lua));
	}

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
	void LuaScript::printError(const std::string& var, const std::string& motive) {
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Can't get (" + var + "): " + motive);
	}
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	void LuaScript::printStack(lua_State* lua) {
		int top = lua_gettop(lua);

		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"printing " + m_path + " script stack...");

		for (int i = 1; i <= top; i++) {
			int type = lua_type(lua, i);

			switch (type) {
			case LUA_TSTRING:
				ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
					"    " + std::string(lua_tostring(lua, i)));
				break;
			case LUA_TBOOLEAN:
				printf(lua_toboolean(lua, i) ? "true" : "false");
				break;
			case LUA_TNUMBER:
				printf("%g", lua_tonumber(lua, i));
				break;
			default:
				printf("%s", lua_typename(lua, type));
				break;
			}
		}

		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"---------------------------------------");
	}
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
}
