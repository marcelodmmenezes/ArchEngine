#include "logger.hpp"
#include <cstdlib>
#include <memory>

// services
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
	std::shared_ptr<Core::Logger<Core::ConsoleLogPolicy>>
		console_logger(new Core::Logger<Core::ConsoleLogPolicy>("log.txt"));

	std::shared_ptr<Core::Logger<Core::FileLogPolicy>>
		file_logger(new Core::Logger<Core::FileLogPolicy>("log.txt"));

	console_logger->setThreadName("mainThread");
	file_logger->setThreadName("mainThread");

	Core::ServiceLocator::provideConsoleLoggingService(console_logger);
	Core::ServiceLocator::provideFileLoggingService(file_logger);

#ifndef NDEBUG
	Core::ServiceLocator::getConsoleLogger()->log<Core::LOG_INFO>(
		"File logger create successfully.");
#endif
}