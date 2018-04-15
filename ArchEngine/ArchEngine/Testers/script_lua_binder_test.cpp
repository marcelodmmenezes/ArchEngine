/*===========================================================================*
 * Arch Engine - "Testers/script_lua_binder_test.cpp"                        *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 15/04/2018                                                       *
 * Last Modified: 15/04/2018                                                 *
 *===========================================================================*/


#include "../Core/engineMacros.hpp"
 
 // Check if this tester is active
#if defined(ARCH_ENGINE_SCRIPT_LUA_BINDER_TEST)

#include "../Utils/serviceLocator.hpp"
#include "../Script/luaBinder.hpp"


using namespace Utils;


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


#endif	// ARCH_ENGINE_SCRIPT_LUA_BINDER_TEST