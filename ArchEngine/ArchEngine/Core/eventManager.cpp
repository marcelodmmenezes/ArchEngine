/*===========================================================================*
 * Arch Engine - "Core/eventManager.hpp                                      *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Based in:                                                                 *
 * - (https://www.codeproject.com/Articles/1169105/                          *
 *    Cplusplus-std-thread-Event-Loop-with-Message-Queue)                    *
 * - Game Coding Complete, 4th edition - Mike McShaffry, David Rez Graham    *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 02/05/2018                                                 *
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

		// Initializes concurrent event queue
		if (!m_concurrent_queue.initialize(
			q_thread_name, q_timer_wait_duration)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not initialize event queue");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		m_state = INITIALIZED;

		return true;
	}

	void EventManager::dispatch(unsigned long max_milliseconds) {
		// Calculates the maximum time we can spend from now
		unsigned long ticks = (unsigned long)Timer::getCurrentTicks();
		unsigned long max_ms = max_milliseconds == ULONG_MAX ?
			ULONG_MAX : ticks + max_milliseconds;

		// Gets events from other threads into event queue
		EventPtr evnt;
		while (m_concurrent_queue.getEvent(evnt)) {
			// Abort event mechanism
			if (m_abort_event && evnt->getType() == m_event_to_abort) {
				m_abort_event = m_abort_all_of_type ? true : false;
				continue;
			}

			m_event_queue.push(evnt);

			ticks = (unsigned long)Timer::getCurrentTicks();

#ifndef ARCH_ENGINE_LOGGER_WARNING
			if (max_milliseconds != ULONG_MAX && ticks >= max_ms)
				ServiceLocator::getFileLogger()->log<LOG_WARNING>(
					"EventManager dispatcher: ConcurrentQueue flooding");
#endif	// ARCH_ENGINE_LOGGER_WARNING
		}

		m_abort_event = false;

		// Dipatches the events
		while (!m_event_queue.empty()) {
			// Gets event from queue
			auto evnt = m_event_queue.front();
			m_event_queue.pop();

			const EventType event_type = evnt->getType();

			// Iterates through registered listeners, invoking them
			auto it = m_event_listeners.find(event_type);
			auto aux_it = it;

			while (it != m_event_listeners.end() &&
				it->first == aux_it->first) {
				it->second.invoke(evnt);
				++it;
			}

			ticks = (unsigned int)Timer::getCurrentTicks();

#ifndef ARCH_ENGINE_LOGGER_WARNING
			if (max_milliseconds != ULONG_MAX && ticks >= max_ms)
				ServiceLocator::getFileLogger()->log<LOG_WARNING>(
					"EventManager dispatcher: "
					"Could not process all frame's events");
#endif	// ARCH_ENGINE_LOGGER_WARNING
		}
	}

	void EventManager::destroy() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		// Destroys concurrent event queue
		m_concurrent_queue.destroy();

		m_state = SAFE_TO_DESTROY;
	}

	bool EventManager::addListener(const Delegate<void(EventPtr)>& listener,
		EventType evnt) {
		auto it = m_event_listeners.find(evnt);
		auto aux_it = it;

		// Iterates through registered listeners,
		// to avoid inserting duplicate listener
		while (it != m_event_listeners.end() &&
			it->first == aux_it->first) {
			if (it->second == listener)
				return false;
			++it;
		}

		m_event_listeners.insert(std::make_pair(evnt, listener));

		return true;
	}

	bool EventManager::removeListener(const Delegate<void(EventPtr)>& listener,
		EventType evnt) {
		auto it = m_event_listeners.find(evnt);
		auto aux_it = it;

		// Iterates through registered listeners
		while (it != m_event_listeners.end() &&
			it->first == aux_it->first) {
			if (it->second == listener) {
				m_event_listeners.erase(it);
				return true;
			}
			++it;
		}

		return false;
	}

	bool EventManager::triggerEvent(EventPtr& evnt) {
		bool triggered = false;

		auto it = m_event_listeners.find(evnt->getType());
		auto aux_it = it;

		// Iterates through registered listeners
		while (it != m_event_listeners.end() &&
			it->first == aux_it->first) {
			it->second.invoke(evnt);
			triggered = true;
			++it;
		}

		return triggered;
	}

	bool EventManager::enqueueEvent(EventPtr& evnt) {
		// Posts event if there are listeners to it.
		if (m_event_listeners.find(evnt->getType())
			!= m_event_listeners.end()) {
			m_concurrent_queue.postEvent(evnt);
			return true;
		}

		return false;
	}

	void EventManager::abortEvent(EventType type, bool all_of_type) {
		m_abort_event = true;
		m_abort_all_of_type = all_of_type;
		m_event_to_abort = type;
	}
}