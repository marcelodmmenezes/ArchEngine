/*===========================================================================*
 * Arch Engine - "Script/engineLuaAPI.cpp                                    *
 *                                                                           *
 * Passes all engine API functions to Lua scripts.                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/05/2018                                                       *
 * Last Modified: 17/05/2018                                                 *
 *===========================================================================*/


#include "engineLuaAPI.hpp"


using namespace Utils;


namespace Script {
	EngineLuaAPI::EngineLuaAPI() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"EngineLuaAPI constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	EngineLuaAPI::~EngineLuaAPI() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"EngineLuaAPI destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	EngineLuaAPI& EngineLuaAPI::getInstance() {
		static EngineLuaAPI instance;
		return instance;
	}

	bool EngineLuaAPI::initialize(const std::string& config_path) {
		LuaScript lua_context;

		LuaFunctions functions;
		functions.push_back(std::make_pair("captureMouse", ::captureMouse));
		functions.push_back(std::make_pair("releaseMouse", ::releaseMouse));
		functions.push_back(std::make_pair("addMesh", ::addMesh));
		functions.push_back(std::make_pair("removeMesh", ::removeMesh));
		functions.push_back(std::make_pair("pushContext", ::pushContext));
		functions.push_back(std::make_pair("popContext", ::popContext));

		if (!lua_context.initialize(config_path, functions))
			return false;

		m_file = config_path;

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		// Register for hot reload
		m_file_modified_listener.bind
			<EngineLuaAPI, &EngineLuaAPI::onFileModifiedEvent>(this);
		Core::EventManager::getInstance().addListener(
			m_file_modified_listener, Core::EVENT_FILE_MODIFIED);

		Core::EventPtr evnt = std::make_shared<WatchFileEvent>(
			WatchFileEvent(m_file, true));
		Core::EventManager::getInstance().sendEvent(evnt);
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

		lua_context.destroy();

		return true;
	}

	void EngineLuaAPI::destroy() {
		// TODO
	}

	void EngineLuaAPI::onFileModifiedEvent(Core::EventPtr e) {
		auto evnt = std::static_pointer_cast<FileModifiedEvent>(e);

		if (evnt->getPath() != m_file)
			return;

		LuaScript lua_context;

		LuaFunctions functions;
		functions.push_back(std::make_pair("pushContext", ::pushContext));
		functions.push_back(std::make_pair("popContext", ::popContext));
		functions.push_back(std::make_pair("addMesh", ::addMesh));
		functions.push_back(std::make_pair("removeMesh", ::removeMesh));
		functions.push_back(std::make_pair("captureMouse", ::captureMouse));
		functions.push_back(std::make_pair("releaseMouse", ::releaseMouse));

		lua_context.initialize(m_file, functions);

		lua_context.destroy();
	}
}