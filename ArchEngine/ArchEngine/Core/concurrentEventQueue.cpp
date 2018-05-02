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


#include "concurrentEventQueue.hpp"


using namespace Utils;


namespace Core {
	ConcurrentEventQueue::ConcurrentEventQueue() :
		m_state(CONSTRUCTED), m_thread(nullptr) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			m_thread_name + " concurrent event queue constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	ConcurrentEventQueue::~ConcurrentEventQueue() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == CONSTRUCTED || m_state == SAFE_TO_DESTROY);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			m_thread_name + " concurrent event queue destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	bool ConcurrentEventQueue::initialize(const std::string& thread_name,
		unsigned timer_wait_duration) {
		m_thread_name = thread_name;
		m_timer_wait_duration = timer_wait_duration;

		if (!m_thread)
			m_thread = new std::thread(&ConcurrentEventQueue::update, this);
		else
			return false;

		m_state = INITIALIZED;

		return true;
	}

	void ConcurrentEventQueue::destroy() {
		if (!m_thread)
			return;

		// std::lock_guard unlocks in destructor, therefore the brackets
		{
			std::lock_guard<std::mutex> lock(m_mq_mutex);

			// Creates and post a detroy message to the messaging system
			m_message_queue.push(std::move(ThreadMessage{
				TMT_DESTROY, nullptr }));

			m_cv.notify_one();
		}

		m_thread->join();
		delete m_thread;
		m_thread = nullptr;

		m_state = SAFE_TO_DESTROY;
	}

	std::thread::id ConcurrentEventQueue::getQueueThreadId() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS
		return m_thread->get_id();
	}

	std::thread::id ConcurrentEventQueue::getCurrentThreadId() {
		return std::this_thread::get_id();
	}

	void ConcurrentEventQueue::postEvent(EventPtr evnt) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		std::unique_lock<std::mutex> lock(m_mq_mutex);

		m_message_queue.push(std::move(ThreadMessage{
			TMT_USER_EVENT, std::move(evnt) }));

		m_cv.notify_one();
	}

	bool ConcurrentEventQueue::getEvent(EventPtr& evnt) {
		m_eq_mutex.lock();

		if (m_event_queue.empty())
			return false;

		evnt = std::move(m_event_queue.front());
		m_event_queue.pop();

		m_eq_mutex.unlock();

		return true;
	}

	void ConcurrentEventQueue::update() {
		m_timer_exit = false;
		std::thread timer_thread(&ConcurrentEventQueue::timerUpdate, this);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->setThreadName(m_thread_name);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		ThreadMessage msg; // Aux to pop from queue

		while (true) {
			// std::unique_lock unlocks in destructor, therefore the brackets
			{
				std::unique_lock<std::mutex> lock(m_mq_mutex);

				// Waiting for event to be added to the queue
				while (m_message_queue.empty())
					m_cv.wait(lock);

				if (m_message_queue.empty())
					continue;

				msg = m_message_queue.front();
				m_message_queue.pop();
			}

			switch (msg.m_type) {
			case TMT_USER_EVENT:
				m_eq_mutex.lock();
				m_event_queue.push(std::move(msg.m_event));
				m_eq_mutex.unlock();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
				ServiceLocator::getFileLogger()->log<LOG_INFO>(
					"TMT_USER_EVENT");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

				break;

			case TMT_TIMER:
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
				ServiceLocator::getConsoleLogger()->log<LOG_INFO>(
					"Timer expired on " + m_thread_name);
				ServiceLocator::getFileLogger()->log<LOG_INFO>(
					"Timer expired on " + m_thread_name);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

				break;

			case TMT_DESTROY:
				m_timer_exit = true;
				timer_thread.join();

				std::unique_lock<std::mutex> lock(m_mq_mutex);

				// Ignoring the remainig messages
				while (!m_message_queue.empty()) {
					msg = m_message_queue.front();
					m_message_queue.pop();
				}

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
				ServiceLocator::getFileLogger()->log<LOG_INFO>(
					"Exit thread on " + m_thread_name);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

				return; // Quits daemon
			}
		}
	}

	void ConcurrentEventQueue::timerUpdate() {
		while (!m_timer_exit) {
			std::this_thread::sleep_for(
				std::chrono::milliseconds(m_timer_wait_duration));

			std::unique_lock<std::mutex> lock(m_mq_mutex);

			m_message_queue.push(std::move(ThreadMessage{
				TMT_TIMER, nullptr }));

			m_cv.notify_one();
		}
	}
}