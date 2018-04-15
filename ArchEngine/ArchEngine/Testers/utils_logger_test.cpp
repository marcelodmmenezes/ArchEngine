/*===========================================================================*
 * Arch Engine - "Testers/utils_logger_test.cpp"                             *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 09/04/2018                                                       *
 * Last Modified: 15/04/2018                                                 *
 *===========================================================================*/


#include "../Core/engineMacros.hpp"

// Check if this tester is active
#if defined(ARCH_ENGINE_UTILS_LOGGER_TEST)

#include "../Utils/logger.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <memory>
#include <string>
#include <thread>


using namespace Utils;


void startLoggingService();
void logDaemon(const std::string& thread_name);


int main(int argc, char* argv[]) {
	try {
		startLoggingService();

		std::thread t1(logDaemon, "t1");
		std::thread t2(logDaemon, "t2");
		std::thread t3(logDaemon, "t3");

		t1.join();
		t2.join();
		t3.join();
	}
	catch (...) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


void startLoggingService() {
	std::shared_ptr<Logger<ConsoleLogPolicy>>
		console_logger(new Logger<ConsoleLogPolicy>(""));

	std::shared_ptr<Logger<FileLogPolicy>>
		file_logger(new Logger<FileLogPolicy>("log.txt"));

	console_logger->setThreadName("mainThread");
	file_logger->setThreadName("mainThread");

	ServiceLocator::provideConsoleLogger(console_logger);
	ServiceLocator::provideFileLogger(file_logger);

	ServiceLocator::getConsoleLogger()->log<LOG_INFO>(
		"Logging Systems online...");
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"Logging Systems online...");
}

void logDaemon(const std::string& thread_name) {
	/*
	auto console_logger = ServiceLocator::getConsoleLogger();
	console_logger->setThreadName(thread_name);
	console_logger->log<LOG_INFO>("Test");
	*/

	for (int i = 0; i < 4000; i++) {
		auto file_logger = ServiceLocator::getFileLogger();
		file_logger->setThreadName(thread_name + "_" + std::to_string(i));
		
		if (thread_name == "t1")
			file_logger->log<LOG_WARNING>("Warning message");
		else if (thread_name == "t2")
			file_logger->log<LOG_ERROR>("FATAL ERROR");
		else
			file_logger->log<LOG_DEBUG>("Han shot first");
	}
}

#endif	// ARCH_ENGINE_UTILS_LOGGER_TEST