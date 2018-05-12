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
 * Last Modified: 11/05/2018                                                 *
 *===========================================================================*/


#ifndef CORE_EVENT_MANAGER_HPP
#define CORE_EVENT_MANAGER_HPP


#include "event.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/delegate.hpp"
#include "../Utils/serviceLocator.hpp"
#include "../Utils/timer.hpp"

#include <cassert>
#include <condition_variable>
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

		bool initialize(const std::string& config_path);
		void dispatch(unsigned long max_milliseconds = ULONG_MAX);
		void destroy();

		bool addListener(const EventListener& listener,
			EventType evnt);
		bool removeListener(const EventListener& listener,
			EventType evnt);

		// Gets this queue m_thread id
		std::thread::id getQueueThreadId();

		// Gets the id of the current executing thread
		static std::thread::id getCurrentThreadId();

		// Posts a message to the queue
		// CAREFUL: Events are moved
		bool postEvent(EventPtr& evnt);

		// Skips queue and call listeners directly
		bool triggerEvent(EventPtr& evnt);
		void abortEvent(EventType type, bool all_of_type = false);

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		EventManager();

		// Queue methods
		void queueDaemon();
		void timerDaemon();

		// EventManager state
		State m_state;

		// Listeners by event type
		std::multimap<EventType, EventListener> m_event_listeners;

		// Events to be dispatched
		std::queue<EventPtr> m_event_queue;

		// Concurrent event queue stores events from any thread.
		std::queue<EventPtr> m_concurrent_queue;

		// Concurrent queue control
		std::thread* m_cq_thread;
		std::string m_cq_thread_name;
		std::mutex m_cq_mutex, m_eq_mutex;
		std::condition_variable m_cq_cv;
		std::atomic<bool> m_timer_exit;

		// Amount of time in milliseconds the timerUpdate should wait
		// before posting a message
		unsigned m_timer_wait_duration;

		// Event abortion flags
		bool m_abort_event;
		bool m_abort_all_of_type;
		EventType m_event_to_abort;
	};
}


#endif