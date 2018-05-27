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
 * Last Modified: 26/05/2018                                                 *
 *===========================================================================*/


#include "luaScript.hpp"


using namespace Utils;


namespace Script {
	LuaScript::LuaScript() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"LuaScript constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	LuaScript::~LuaScript() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == CONSTRUCTED || m_state == SAFE_TO_DESTROY);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"LuaScript destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	bool LuaScript::initialize(const std::string& path,
		const LuaFunctions& functions) {
		m_path = path;

		m_lua = luaL_newstate();
		luaL_openlibs(m_lua);

		// Push functions to script
		for (auto& it : functions)
			lua_register(m_lua, it.first.c_str(), it.second);


		if (luaL_loadfile(m_lua, m_path.c_str()) ||
			lua_pcall(m_lua, 0, 0, 0)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Failed to load(" + m_path + "): " + lua_tostring(m_lua, -1));
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			clearStack();
			m_lua = nullptr;
			return false;
		}

		m_state = INITIALIZED;
		return true;
	}

	void LuaScript::destroy() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		if (m_lua)
			lua_close(m_lua);

		m_state = SAFE_TO_DESTROY;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			m_path + " lua script destroyed");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	void LuaScript::clearStack() {
		lua_pop(m_lua, lua_gettop(m_lua));
	}

	std::vector<int> LuaScript::getIntVector(const std::string& name) {
		std::vector<int> v;
		int discard;

		getToStack(name.c_str(), discard);
		if (lua_isnil(m_lua, -1))
			return std::vector<int>();

		lua_pushnil(m_lua);
		while (lua_next(m_lua, -2)) {
			v.push_back((int)lua_tonumber(m_lua, -1));
			lua_pop(m_lua, 1);
		}

		clearStack();
		return v; // Moved, not copied
	}

	std::vector<float> LuaScript::getFloatVector(const std::string& name) {
		std::vector<float> v;
		int discard;

		getToStack(name.c_str(), discard);
		if (lua_isnil(m_lua, -1))
			return std::vector<float>();

		lua_pushnil(m_lua);
		while (lua_next(m_lua, -2)) {
			v.push_back((float)lua_tonumber(m_lua, -1));
			lua_pop(m_lua, 1);
		}

		clearStack();
		return v; // Moved, not copied
	}

	std::vector<std::string> LuaScript::getStringVector(const std::string& name) {
		std::vector<std::string> v;
		int discard;

		getToStack(name.c_str(), discard);
		if (lua_isnil(m_lua, -1))
			return std::vector<std::string>();

		lua_pushnil(m_lua);
		while (lua_next(m_lua, -2)) {
			v.push_back(lua_tostring(m_lua, -1));
			lua_pop(m_lua, 1);
		}

		clearStack();
		return v; // Moved, not copied
	}

	std::vector<std::string> LuaScript::getTableKeys(const std::string& name) {
		std::string code =
			"function getKeys(name) "
			"    s = \"\""
			"    for k, v in pairs(_G[name]) do "
			"        s = s..k..\",\" "
			"    end "
			"    return s "
			"end"; // Lua function for getting table keys

		luaL_loadstring(m_lua, code.c_str()); // load code
		lua_pcall(m_lua, 0, 0, 0); // execute code
		lua_getglobal(m_lua, "getKeys"); // get function
		lua_pushstring(m_lua, name.c_str());
		lua_pcall(m_lua, 1, 1, 0); // execute function

		std::string test = lua_tostring(m_lua, -1);
		std::vector<std::string> strings;
		std::string temp = "";

		for (unsigned i = 0; i < test.size(); i++) {
			if (test.at(i) != ',')
				temp += test.at(i);
			else {
				strings.push_back(temp);
				temp = "";
			}
		}

		clearStack();
		return strings; // Moved, not copied
	}

	std::vector<std::pair<std::string, std::string>>
		LuaScript::getTablePairs(const std::string& name) {
		std::string code =
			"function getPairs(name) "
			"    s = \"\" "
			"    for k, v in pairs(name) do "
			"        s = s..k..\":\"..tostring(v)..\",\" "
			"    end "
			"    return s "
			"end";

		luaL_loadstring(m_lua, code.c_str()); // load code
		lua_pcall(m_lua, 0, 0, 0); // execute code
		lua_getglobal(m_lua, "getPairs"); // get function

		std::vector<std::string> tables;
		std::string table_name = "";
		for (unsigned i = 0; i < name.size(); i++) {
			if (name.at(i) == '.') {
				tables.push_back(table_name);
				table_name = "";
			}
			else
				table_name += name.at(i);
		}

		tables.push_back(table_name);

		lua_getglobal(m_lua, tables[0].c_str());
		for (unsigned i = 1; i < tables.size(); i++) {
			lua_getfield(m_lua, -1, tables[i].c_str());
			lua_remove(m_lua, -2);
		}

		lua_pcall(m_lua, 1, 1, 0); // execute function

		std::string test = lua_tostring(m_lua, -1);
		std::vector<std::pair<std::string, std::string>> pairs;
		std::string tempk = "", tempv = "";

		for (unsigned i = 0; i < test.size(); i++) {
			if (test.at(i) == ':') {
				tempk = tempv;
				tempv = "";
			}
			else if (test.at(i) != ',')
				tempv += test.at(i);
			else {
				pairs.push_back(std::make_pair(tempk, tempv));
				tempv = "";
			}
		}

		clearStack();
		return pairs; // Moved, not copied
	}

	bool LuaScript::getToStack(const std::string& var_name, int& level) {
		level = 0;

		std::string var = "";

		for (unsigned i = 0; i < var_name.size(); i++) {
			if (var_name.at(i) == '.') {
				if (level == 0)
					lua_getglobal(m_lua, var.c_str());
				else
					lua_getfield(m_lua, -1, var.c_str());

				if (lua_isnil(m_lua, -1)) {
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
			lua_getglobal(m_lua, var.c_str());
		else
			lua_getfield(m_lua, -1, var.c_str());

		if (lua_isnil(m_lua, -1)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			printError(var_name, var + " is not defined");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		return true;
	}

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
	void LuaScript::printError(const std::string& var, const std::string& motive) {
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Can't get (" + var + "): " + motive);
	}
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	void LuaScript::printStack() {
		int top = lua_gettop(m_lua);

		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"printing " + m_path + " script stack...");

		for (int i = 1; i <= top; i++) {
			int type = lua_type(m_lua, i);

			switch (type) {
			case LUA_TSTRING:
				ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
					"    " + std::string(lua_tostring(m_lua, i)));
				break;
			case LUA_TBOOLEAN:
				ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
					"    " + lua_toboolean(m_lua, i) ? "true" : "false");
				break;
			case LUA_TNUMBER:
				ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
					std::to_string(lua_tonumber(m_lua, i)));
				break;
			default:
				ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
					lua_typename(m_lua, type));
				break;
			}
		}

		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"---------------------------------------");
	}
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
}
