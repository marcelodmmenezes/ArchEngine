/*===========================================================================*
 * Arch Engine - "Core/systemManager.hpp"                                    *
 *                                                                           *
 * Heart of the engine. Responsible for systems intialization and shutdown,  *
 * the game loop and other stuff I'll add as the egine grows...              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/04/2018                                                       *
 * Last Modified: 25/04/2018                                                 *
 *===========================================================================*/


#include "systemManager.hpp"


using namespace Utils;


namespace Core {
	SystemManager::SystemManager() : m_initialized(false) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"System Manager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	}

	SystemManager::~SystemManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"System Manager destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	}

	SystemManager& SystemManager::getInstance() {
		static SystemManager instance;
		return instance;
	}

	bool SystemManager::initialize() {
		// Prevents the engine to be initalized more than once
		if (m_initialized) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Trying to initialize the engine again");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

			assert(false);
		}

		// TODO

		m_initialized = true;
	}

	void SystemManager::run() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Running main loop");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		// TODO
	}

	int SystemManager::exit() {
		// TODO
	}

	bool SystemManager::isInitialized() const { return m_initialized; }
}