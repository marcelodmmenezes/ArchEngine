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
 * Last Modified: 13/05/2018                                                 *
 *===========================================================================*/


#include "eventManager.hpp"


using namespace Script;
using namespace Utils;


namespace Core {
	EventManager::EventManager() :
		m_state(CONSTRUCTED), m_cq_thread(nullptr) {
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

		m_cq_thread_name = lua_context.get<std::string>("queueThreadName");
		m_timer_wait_duration =
			(unsigned)lua_context.get<int>("queueTimerWaitDuration");

		lua_context.destroy();

		if (!m_cq_thread)
			m_cq_thread = new std::thread(&EventManager::queueDaemon, this);
		else
			return false;

		m_state = INITIALIZED;

		return true;
	}

	void EventManager::dispatch(unsigned long max_milliseconds) {
		// Calculates the maximum time we can spend from now
		unsigned long ticks = (unsigned long)Timer::getCurrentTicks();
		unsigned long max_ms = max_milliseconds == ULONG_MAX ?
			ULONG_MAX : ticks + max_milliseconds;

		m_eq_mutex.lock();

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

		m_eq_mutex.unlock();
	}

	void EventManager::destroy() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		// Removes listeners
		m_event_listeners.clear();

		// Destroys concurrent event queue
		if (!m_cq_thread) {
			m_state = SAFE_TO_DESTROY;
			return;
		}

		// std::lock_guard unlocks in destructor, therefore the brackets
		{
			std::lock_guard<std::mutex> lock(m_cq_mutex);

			// Creates and post a detroy message to the messaging system
			EventPtr evnt = std::make_shared<CoreQuitEvent>(CoreQuitEvent());
			m_concurrent_queue.push(evnt);

			m_cq_cv.notify_one();
		}

		m_cq_thread->join();
		delete m_cq_thread;
		m_cq_thread = nullptr;

		m_state = SAFE_TO_DESTROY;
	}

	bool EventManager::addListener(const EventListener& listener,
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

	std::thread::id EventManager::getQueueThreadId() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS
		return m_cq_thread->get_id();
	}

	std::thread::id EventManager::getCurrentThreadId() {
		return std::this_thread::get_id();
	}

	bool EventManager::postEvent(EventPtr& evnt) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		// Posts event if there are listeners to it.
		if (m_event_listeners.find(evnt->getType())
			!= m_event_listeners.end()) {
			std::unique_lock<std::mutex> lock(m_cq_mutex);

			m_concurrent_queue.push(evnt);

			m_cq_cv.notify_one();

			return true;
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

	void EventManager::abortEvent(EventType type, bool all_of_type) {
		m_abort_event = true;
		m_abort_all_of_type = all_of_type;
		m_event_to_abort = type;
	}

	void EventManager::queueDaemon() {
		m_timer_exit = false;
		std::thread timer_thread(&EventManager::timerDaemon, this);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->setThreadName(m_cq_thread_name);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		EventPtr evnt;

		while (true) {
			// std::unique_lock unlocks in destructor, therefore the brackets
			{
				std::unique_lock<std::mutex> lock(m_cq_mutex);

				// Waiting for event to be added to the queue
				while (m_concurrent_queue.empty())
					m_cq_cv.wait(lock);

				if (m_concurrent_queue.empty())
					continue;

				evnt = m_concurrent_queue.front();
				m_concurrent_queue.pop();
			}

			if (evnt->getType() == EVENT_CORE_TIMER) {
				/*
				auto e = std::static_pointer_cast<CoreTimerEvent>(evnt);
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
				ServiceLocator::getFileLogger()->log<LOG_INFO>(
					std::to_string(e->getTime()));
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
				*/
			}
			else {
				m_eq_mutex.lock();
				m_event_queue.push(evnt);
				m_eq_mutex.unlock();
			}

			if (evnt->getType() == EVENT_CORE_QUIT) {
				m_timer_exit = true;
				timer_thread.join();

				std::unique_lock<std::mutex> lock(m_cq_mutex);

				// Ignoring the remainig messages
				while (!m_concurrent_queue.empty())
					m_concurrent_queue.pop();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
				ServiceLocator::getFileLogger()->log<LOG_INFO>(
					"Exit thread on " + m_cq_thread_name);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

				return; // Quits daemon
			}
		}
	}

	void EventManager::timerDaemon() {
		while (!m_timer_exit) {
			std::this_thread::sleep_for(
				std::chrono::milliseconds(m_timer_wait_duration));

			std::unique_lock<std::mutex> lock(m_cq_mutex);

			// Creates and post a detroy message to the messaging system
			EventPtr evnt = std::make_shared<CoreTimerEvent>(
				CoreTimerEvent(m_timer_wait_duration));
			m_concurrent_queue.push(evnt);

			m_cq_cv.notify_one();
		}
	}
}