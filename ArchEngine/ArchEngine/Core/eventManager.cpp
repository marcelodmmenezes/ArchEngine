/*===========================================================================*
 * Arch Engine - "Core/eventManager.cpp                                      *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#include "eventManager.hpp"


using namespace Script;
using namespace Utils;


namespace Core {
	EventManager::EventManager() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"EventManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	EventManager::~EventManager() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == CONSTRUCTED || m_state == SAFE_TO_DESTROY);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"EventManager destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	EventManager& EventManager::getInstance() {
		static EventManager instance;
		return instance;
	}

	bool EventManager::initialize(const std::string& config_path) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == CONSTRUCTED || m_state == SAFE_TO_DESTROY);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		LuaScript lua_context;
		lua_context.initialize(config_path);

		std::string q_thread_name =
			lua_context.get<std::string>("queueThreadName");
		unsigned q_timer_wait_duration =
			(unsigned)lua_context.get<int>("queueTimerWaitDuration");

		lua_context.destroy();

		// Initializes event queue
		if (!m_queue.initialize(q_thread_name, q_timer_wait_duration)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not initialize event queue");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		m_state = INITIALIZED;

		return true;
	}

	void EventManager::destroy() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS


		// Destroys event queue
		m_queue.destroy();

		m_state = SAFE_TO_DESTROY;
	}
}