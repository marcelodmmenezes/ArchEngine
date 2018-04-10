/*===========================================================================*
 * Arch Engine - "Utils/logger.hpp"                                          *
 *                                                                           *
 * Simple, lightweight, line-level thread safe logging system. The system is *
 * more concerned about modularity than cache coherence, once logging should *
 * be almost completely deactivated in Arch Engine's release version.        *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 08/04/2018                                                       *
 * Last Modified: 10/04/2018                                                 *
 *===========================================================================*/


#ifndef UTILS_LOGGER_HPP
#define UTILS_LOGGER_HPP


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


namespace Utils {

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
			m_output_stream.open(path.c_str(), std::ios_base::binary |
				std::ios_base::out | std::ios_base::app);

			if (!m_output_stream.is_open())
				return false;

#if defined(__unix__)
			m_output_stream << "-----------------------------------------------\
----------------- STARTED LOGGING\n";
#elif defined(_MSC_VER)
			m_output_stream << "-----------------------------------------------\
----------------- STARTED LOGGING\r\n";
#endif

			return true;
		}

		void closeOutputStream() override {
			if (m_output_stream.is_open()) {
#if defined(__unix__)
				m_output_stream << "-------------------------------------------\
----------------------- ENDED LOGGING\n" << std::endl;
#elif defined(_MSC_VER)
				m_output_stream << "-------------------------------------------\
----------------------- ENDED LOGGING\r\n" << std::endl;
#endif

				m_output_stream.close();
			}
		}

		void write(const std::string& msg) override {
			m_output_stream << msg << std::endl;
		}

	private:
		std::ofstream m_output_stream;
	};


	//------------------------------------------------------------------- Logger
	template<typename LogPolicy> class Logger;

	// The logging service runs on the background through this daemon.
	// It uses a unique_lock with timed_mutex to avoid simultaneous access to
	// the data by multiple threads.
	template <typename LogPolicy>
	void loggingDaemon(Logger<LogPolicy>* logger) {
		do {
			if (logger->m_log_buffer.size()) {
				logger->m_write_mutex.lock();

				for (auto &it : logger->m_log_buffer)
					logger->m_policy.write(it);

				logger->m_log_buffer.clear();
				logger->m_write_mutex.unlock();
			}
		}
		while (logger->m_is_still_running.test_and_set() ||
			logger->m_log_buffer.size());
	}

	template<typename LogPolicy>
	class Logger {
	public:
		Logger(const std::string& name);
		~Logger();

		void setThreadName(const std::string& name);

		template<LogLevel level>
		void log(const std::stringstream& stream);

		template<LogLevel level>
		void log(const std::string& msg);

		template<typename Policy>
		friend void loggingDaemon(Logger<Policy>* logger);

	private:
		// Logging stuff
		LogPolicy m_policy;
		unsigned m_log_line_number;
		std::map<std::thread::id, std::string> m_thread_names;
		std::vector<std::string> m_log_buffer;

		// Concurrency stuff
		std::mutex m_write_mutex;
		std::thread m_daemon;
		std::atomic_flag m_is_still_running{ ATOMIC_FLAG_INIT };
	};


	template<typename LogPolicy>
	Logger<LogPolicy>::Logger(const std::string& path) : m_log_line_number(0u) {
		if (m_policy.openOutputStream(path)) {
			m_is_still_running.test_and_set();
			m_daemon = std::move(std::thread{ loggingDaemon<LogPolicy>, this });
		}
		else {
#if defined(__unix__)
			throw std::runtime_error("Unable to open the file "
				+ path + " for logging\n");
#elif defined(_MSC_VER)
			throw std::runtime_error("Unable to open the file "
				+ path + " for logging\r\n");
#endif
		}
	}

	template<typename LogPolicy>
	Logger<LogPolicy>::~Logger() {
#ifndef NDEBUG
		// Log closing message
		log<LOG_INFO>("Shutting down Logging Systems");
#endif	// NDEBUG

		// Stop the daemon thread
		m_is_still_running.clear();
		m_daemon.join();

		// Clearing and releasing memory from the containers
		m_thread_names.clear();
		std::map<std::thread::id, std::string>().swap(m_thread_names);

		m_log_buffer.clear();
		m_log_buffer.shrink_to_fit();

		m_policy.closeOutputStream();
	}

	template<typename LogPolicy>
	void Logger<LogPolicy>::setThreadName(const std::string& name) {
		m_thread_names[std::this_thread::get_id()] = name;
	}

	template<typename LogPolicy>
	template<LogLevel level>
	void Logger<LogPolicy>::log(const std::stringstream& stream) {
		std::stringstream log_stream;

		// Writes line number and date/time
		time_t time;
		std::tm bt;
		char buffer[100];

		std::time(&time);

		// localtime is thread-unsafe, both unix and windows have alternatives
#if defined(__unix__)
		localtime_r(&time, &bt);
		std::strftime(buffer, 100, "%d/%m/%Y - %T", &bt);
#elif defined(_MSC_VER)
		localtime_s(&bt, &time);
		std::strftime(buffer, 100, "%d/%m/%Y - %T", &bt);
#endif

		m_write_mutex.lock();

		log_stream << m_log_line_number++ << ": " << buffer << "    ";

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
		log_stream << m_thread_names[std::this_thread::get_id()] << ":    ";

		// Writes the message
		log_stream << stream.str();

		m_log_buffer.push_back(log_stream.str());

		m_write_mutex.unlock();
	}

	template<typename LogPolicy>
	template<LogLevel level>
	void Logger<LogPolicy>::log(const std::string& msg) {
		std::stringstream stream;
		stream << msg.c_str();
		log<level>(stream);
	}
}


#endif	// UTILS_LOGGER_HPP