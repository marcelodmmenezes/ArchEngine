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


#ifndef CORE_EVENT_MANAGER_HPP
#define CORE_EVENT_MANAGER_HPP


#include "event.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/delegate.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cassert>
#include <limits>
#include <map>
#include <queue>
#include <string>
#include <vector>


namespace Core {
	class EventManager {
	public:
		~EventManager();

		EventManager(const EventManager&) = delete;
		void operator=(const EventManager&) = delete;

		static EventManager& getInstance();

		bool addListener(const EventListener& listener,
			EventType evnt);
		bool removeListener(const EventListener& listener,
			EventType evnt);

		// Posts a message to the queue
		// CAREFUL: Events are moved
		bool sendEvent(EventPtr& evnt);

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		EventManager();

		// EventManager state
		State m_state;

		// Listeners by event type
		std::multimap<EventType, EventListener> m_event_listeners;
	};
}


#endif