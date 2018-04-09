/*============================================================================*
 * Arch Engine - "Core/logger.hpp"                                            *
 *                                                                            *
 * Simple, lightweight, line-level thread safe logging system. The system is  *
 * more concerned about modularity than cache coherence, once logging should  *
 * be almost completely deactivated in Arch Engine's release version.         *
 *                                                                            *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                      *
 * Created: 08/04/2018                                                        *
 * Last Modified: 08/04/2018                                                  *
 *============================================================================*/


#ifndef CORE_LOGGER_HPP
#define CORE_LOGGER_HPP


#include "serviceLocator.hpp"

#include <atomic>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


namespace Core {

	// The Logger class verbosity is based upon its LogLevel type.
	// The LogLevel enum names are self explanatory.
	enum LogLevel {
		LOG_INFO,
		LOG_WARNING,
		LOG_ERROR,
		LOG_DEBUG,
	};


	//------------------------------------------------------------- Log Policies
	// A LogPolicy is used by the Logger to define where the log
	// output should be directed to.
	class LogPolicyInterface {
	public:
		virtual ~LogPolicyInterface() = default;

		virtual bool openOutputStream(const std::string& path) = 0;
		virtual void closeOutputStream() = 0;
		virtual void write(const std::string& msg) = 0;
	};

	// LogPolicy for console writing
	class ConsoleLogPolicy : public LogPolicyInterface {
	public:
		~ConsoleLogPolicy() {}

		bool openOutputStream(const std::string& path) override {
			return true;
		}

		void closeOutputStream() override {}
		
		void write(const std::string& msg) override {
			std::cout << msg << std::endl;
		}
	};

	// LogPolicy for file writing
	class FileLogPolicy : public LogPolicyInterface {
	public:
		FileLogPolicy() {}
		
		~FileLogPolicy() {
			closeOutputStream();
		}

		bool openOutputStream(const std::string& path) override {
			m_output_stream.open(path.c_str(),
				std::ios_base::binary | std::ios_base::out);

			if (!m_output_stream.is_open())
				return false;

			return true;
		}

		void closeOutputStream() override  {
			if (m_output_stream.is_open())
				m_output_stream.close();
		}
		
		void write(const std::string& msg) override {
			m_output_stream << msg << std::endl;
		}

	private:
		std::ofstream m_output_stream;
	};


	//------------------------------------------------------------------- Logger
	template<typename LogPolicy>
	class Logger {
	public:
		Logger(const std::string& name);
		~Logger();

		void setThreadName(const std::string& name);

		template<LogLevel level>
		void log(std::stringstream stream);

		template<LogLevel level>
		void log(std::string msg);

		template<typename Policy>
		friend void loggingDaemon(Logger<Policy>* logger);

	private:
		// Logging stuff
		LogPolicy m_policy;
		unsigned m_log_line_number;
		std::vector<std::string> m_log_buffer;

		// Concurrency stuff
		std::map<std::thread::id, std::string> m_thread_names;
		std::timed_mutex m_write_mutex;
		std::thread m_daemon;
		std::atomic_flag m_is_still_running { ATOMIC_FLAG_INIT };
	};


	template<typename LogPolicy>
	Logger<LogPolicy>::Logger(const std::string& path) : m_log_line_number(0u) {
		if (m_policy.openOutputStream(path)) {
			m_is_still_running.test_and_set();
			m_daemon = std::move(std::thread { loggingDaemon<LogPolicy>, this});
		}
		else throw std::runtime_error("Unable to open the file " +
			path + " for logging\n");
	}

	template<typename LogPolicy>
	Logger<LogPolicy>::~Logger() {
#ifndef NDEBUG
		// Log closing message
		ServiceLocator::getConsoleLogger()->log<LOG_INFO>(
			"Closing Logging System.");
#endif	// NDEBUG

		m_is_still_running.clear();
		m_daemon.join();

		// Clearing and releasing memory from the containers
		m_thread_names.clear();
		std::map<std::thread::id, std::string>().swap(m_thread_names);

		m_log_buffer.clear();
		m_log_buffer.shrink_to_fit();

		m_policy.closeOutputstream();
	}

	template<typename LogPolicy>
	void Logger<LogPolicy>::setThreadName(const std::string& name) {
		m_thread_names[std::this_thread::getId()] = name;
	}

	template<typename LogPolicy>
	template<LogLevel level>
	void Logger<LogPolicy>::log(std::stringstream stream) {
		std::stringstream log_stream;		

		// If file is not empty start writing in a new line
		if (m_log_line_number != 0u)
			log_stream << "\n";

		// Writes line number and date/time
		log_stream << m_log_line_number++ <<
			std::chrono::system_clock::now() << "\t";

		// Writes log level
		switch (level) {
			case LOG_INFO:
				log_stream << "<INFO>: ";
				break;
			case LOG_WARNING:
				log_stream << "<WARNING>: ";
				break;
			case LOG_ERROR:
				log_stream << "<ERROR>: ";
				break;
			case LOG_DEBUG:
				log_stream << "<DEBUG>: ";
				break;
		}

		// Writes thread name
		log_stream << m_thread_names[std::this_thread::getId()] << ":\t";

		// Writes the message
		log_stream << stream.str();
		std::lock_guard<std::timed_mutex> lock(m_write_mutex);
		m_log_buffer.push_back(log_stream.str());
	}

	template<typename LogPolicy>
	template<LogLevel level>
	void Logger<LogPolicy>::log(std::string msg) {
		std::stringstream stream;
		stream << msg.c_str();
		log<level>(stream);
	}

	// The logging service runs on the background through this daemon.
	// It uses a unique_lock with timed_mutex to avoid simultaneous access to
	// the data by multiple threads.
	template <typename LogPolicy>
	void loggingDaemon(Logger<LogPolicy>* logger) {
		std::unique_lock<std::timed_mutex>
			lock(logger->m_write_mutex, std::defer_lock);

		do {
			std::this_thread::sleep_for(std::chrono::milliseconds { 50 });

			if (logger->m_log_buffer.size()) {
				if (!lock.try_lock_for(std::chrono::milliseconds { 50 }))
					continue;

				for (auto &it : logger->m_log_buffer)
					logger->policy.write(it);

				logger->m_log_buffer.clear();
				lock.unlock();
			}
		}
		while(logger->m_is_still_running.test_and_set() ||
			logger->m_log_buffer.size());
	}
}


#endif	// CORE_LOGGER_HPP