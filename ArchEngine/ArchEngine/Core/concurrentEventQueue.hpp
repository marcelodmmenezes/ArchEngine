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
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#ifndef CORE_CONCURRENT_EVENT_QUEUE_HPP
#define CORE_CONCURRENT_EVENT_QUEUE_HPP


#include "iEvent.hpp"
#include "../Utils/serviceLocator.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
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
		std::shared_ptr<IEvent> m_event;
	};

	class ConcurrentEventQueue {
	public:
		ConcurrentEventQueue(const std::string& thread_name,
			unsigned timer_wait_duration);

		ConcurrentEventQueue(const ConcurrentEventQueue&) = delete;
		void operator=(const ConcurrentEventQueue&) = delete;

		~ConcurrentEventQueue();

		bool initialize();
		void destroy();

		// Gets this queue m_thread id
		std::thread::id getQueueThreadId();

		// Gets the id of the current executing thread
		static std::thread::id getCurrentThreadId();
		
		// Posts a message to the queue
		// CAREFUL: Events are moved
		// Should be called with std::move
		// e.g.:
		//   std::unique_ptr<IEvent> evnt(new Event());
		//   postEvent(std::move(evnt));
		void postEvent(std::shared_ptr<IEvent> evnt);

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
		std::queue<ThreadMessage> m_queue;
		std::mutex m_mutex;
		std::condition_variable m_cv;
		std::atomic<bool> m_timer_exit;
		std::string m_thread_name;

		// Amount of time in milliseconds the timerUpdate should wait
		// before
		unsigned m_timer_wait_duration;
	};
}


#endif	// CORE_CONCURRENT_EVENT_QUEUE_HPP