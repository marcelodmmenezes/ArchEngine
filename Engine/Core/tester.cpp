#include "logger.hpp"
#include "serviceLocator.hpp"

#include <cstdlib>
#include <memory>


using namespace Core;


void startLoggingService();

int main(int argc, char* argv[]) {
	try {
		startLoggingService(); 
	}
	catch (std::runtime_error) {
		return EXIT_FAILURE;
	}

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
		"File logger create successfully.");
#endif
}