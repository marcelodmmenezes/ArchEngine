#include "logger.hpp"
#include "serviceLocator.hpp"

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

	logDaemon("mainThread");

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
	auto console_logger = ServiceLocator::getConsoleLogger();
	console_logger->setThreadName(thread_name);
	console_logger->log<LOG_INFO>("Test");

	auto file_logger = ServiceLocator::getFileLogger();
	file_logger->setThreadName(thread_name);
	file_logger->log<LOG_INFO>("Test");
}
