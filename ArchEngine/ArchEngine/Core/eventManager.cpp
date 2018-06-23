/*===========================================================================*
 * Arch Engine - "Core/eventManager.hpp                                      *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * For now, it consists of a basic observer pattern implementation.          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 17/05/2018                                                 *
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

	bool EventManager::addListener(const EventListener& listener,
		EventType evnt) {
		auto it = m_event_listeners.find(evnt);
		auto aux_it = it;
/*
		// Iterates through registered listeners,
		// to avoid inserting duplicate listener
		while (it != m_event_listeners.end() &&
			it->first == aux_it->first) {
			if (it->second == listener)
				return false;
			++it;
		}
*/
		m_event_listeners.insert(std::make_pair(evnt, listener));

		return true;
	}

	bool EventManager::removeListener(const EventListener& listener,
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

	bool EventManager::sendEvent(EventPtr& evnt) {
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
}