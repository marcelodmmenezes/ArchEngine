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
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#ifndef CORE_EVENT_MANAGER_HPP
#define CORE_EVENT_MANAGER_HPP


#include "iEvent.hpp"
#include "concurrentEventQueue.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/delegate.hpp"
#include "../Utils/serviceLocator.hpp"
#include "../Utils/timer.hpp"

#include <cassert>
#include <limits>
#include <map>
#include <string>
#include <vector>


namespace Core {
	class EventManager {
	public:
		~EventManager();

		EventManager(const EventManager&) = delete;
		void operator=(const EventManager&) = delete;

		static EventManager& getInstance();

		bool initialize(const std::string& config_path);
		void dispatch(unsigned long max_milliseconds = ULONG_MAX);
		void destroy();

		bool addListener(const Utils::Delegate<void(EventPtr)>& listener,
			EventType evnt);
		bool removeListener(const Utils::Delegate<void(EventPtr)>& listener,
			EventType evnt);

		// Skips queue and call listeners directly
		bool triggerEvent(EventPtr& evnt);
		bool enqueueEvent(EventPtr& evnt);
		void abortEvent(EventType type, bool all_of_type = false);

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		EventManager();

		// EventManager state
		State m_state;

		// Thread-safe realtime event queue.
		// Acquires events from any thread.
		ConcurrentEventQueue m_concurrent_queue;

		// Actual event queue
		std::queue<EventPtr> m_event_queue;

		// Listeners by event type
		std::multimap<EventType, Utils::Delegate<void(EventPtr)>>
			m_event_listeners;

		// Event abortion flags
		bool m_abort_event;
		bool m_abort_all_of_type;
		EventType m_event_to_abort;
	};
}


#endif