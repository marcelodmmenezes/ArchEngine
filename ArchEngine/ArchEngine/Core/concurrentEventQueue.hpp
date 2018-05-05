/*===========================================================================*
 * Arch Engine - "Core/concurrentEventQueue.hpp                              *
 *                                                                           *
 * Thread-safe event queue for the multi-threaded event system.              *
 *                                                                           *
 * Based in:                                                                 *
 * - (https://www.codeproject.com/Articles/1169105/                          *
 *    Cplusplus-std-thread-Event-Loop-with-Message-Queue)                    *
 * - Game Coding Complete, 4th edition - Mike McShaffry, David Rez Graham    *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 05/05/2018                                                 *
 *===========================================================================*/


#ifndef CORE_CONCURRENT_EVENT_QUEUE_HPP
#define CORE_CONCURRENT_EVENT_QUEUE_HPP


#include "event.hpp"
#include "../Utils/serviceLocator.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>


namespace Core {
	enum ThreadMessageType {
		TMT_USER_EVENT, // User defined event
		TMT_TIMER, // Thread timer
		TMT_DESTROY // Stop the queue
	};

	struct ThreadMessage {
		ThreadMessageType m_type;
		EventPtr m_event;
	};

	class ConcurrentEventQueue {
	public:
		ConcurrentEventQueue();

		ConcurrentEventQueue(const ConcurrentEventQueue&) = delete;
		void operator=(const ConcurrentEventQueue&) = delete;

		~ConcurrentEventQueue();

		// Default timer_wait_duration is set to 250 to avoid flooding
		// the queue with TMT_TIMER events
		bool initialize(const std::string& thread_name,
			unsigned timer_wait_duration = 250);
		void destroy();

		// Gets this queue m_thread id
		std::thread::id getQueueThreadId();

		// Gets the id of the current executing thread
		static std::thread::id getCurrentThreadId();
		
		// Posts a message to the queue
		// CAREFUL: Events are moved
		void postEvent(EventPtr evnt);

		// Used by eventManager to dispatch the concurrent events
		// Returns false if m_event_queue is empty
		bool getEvent(EventPtr& evnt);

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		void update();
		void timerUpdate();

		State m_state;

		std::thread* m_thread;
		
		// This queue stores any type of messages from any thread.
		std::queue<ThreadMessage> m_message_queue;
		std::mutex m_mq_mutex;
		// The filtered events are stored in this queue and sent to the
		// event manager for dispatching.
		std::queue<EventPtr> m_event_queue;
		std::mutex m_eq_mutex;

		std::condition_variable m_cv;
		std::atomic<bool> m_timer_exit;
		std::string m_thread_name;

		// Amount of time in milliseconds the timerUpdate should wait
		// before
		unsigned m_timer_wait_duration;
	};
}


#endif	// CORE_CONCURRENT_EVENT_QUEUE_HPP