/*===========================================================================*
 * Arch Engine - "Utils/logger_tester.cpp"                                   *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 09/04/2018                                                       *
 * Last Modified: 10/04/2018                                                 *
 *===========================================================================*/


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
	}
	catch (std::runtime_error) {
		return EXIT_FAILURE;
	}

	std::thread t1(logDaemon, "t1");
	std::thread t2(logDaemon, "t2");
	std::thread t3(logDaemon, "t3");

	t1.join();
	t2.join();
	t3.join();

	return EXIT_SUCCESS;
}

void startLoggingService() {
	std::shared_ptr<Logger<ConsoleLogPolicy>>
		console_logger(new Logger<ConsoleLogPolicy>("log.txt"));

	std::shared_ptr<Logger<FileLogPolicy>>
		file_logger(new Logger<FileLogPolicy>("log.txt"));

	console_logger->setThreadName("mainThread");
	file_logger->setThreadName("mainThread");

	ServiceLocator::provideConsoleLogger(console_logger);
	ServiceLocator::provideFileLogger(file_logger);

#ifndef NDEBUG
	ServiceLocator::getConsoleLogger()->log<LOG_INFO>(
		"Logging Systems online...");
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"Logging Systems online...");
#endif
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
