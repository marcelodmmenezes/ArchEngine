/*===========================================================================*
 * Arch Engine - "Script/engineLuaAPI.hpp                                    *
 *                                                                           *
 * Passes all engine API functions to Lua scripts.                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/05/2018                                                       *
 * Last Modified: 19/05/2018                                                 *
 *===========================================================================*/


#ifndef SCRIPT_ENGINE_LUA_API_HPP
#define SCRIPT_ENGINE_LUA_API_HPP


#include "../Core/engine.hpp"
#include "../Core/eventManager.hpp"
#include "../Graphics/graphicsManager.hpp"
#include "../OS/inputManager.hpp"
#include "luaScript.hpp"
#include "../Utils/fileWatcher.hpp"
#include "../Utils/serviceLocator.hpp"

#include <string>


namespace Script {
	class EngineLuaAPI {
	public:
		~EngineLuaAPI();

		EngineLuaAPI(const EngineLuaAPI&) = delete;
		void operator=(const EngineLuaAPI&) = delete;

		static EngineLuaAPI& getInstance();

		bool initialize(const std::string& config_path);
		void destroy();

		void onFileModifiedEvent(Core::EventPtr e);

	private:
		EngineLuaAPI();

		std::string m_file;

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		Core::EventListener m_file_modified_listener;
#endif	// ARCH_ENGINE_HOT_RELOAD_ON
	};
}


#endif	// SCRIPT_ENGINE_LUA_API_HPP