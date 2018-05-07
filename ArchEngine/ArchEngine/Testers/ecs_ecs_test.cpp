/*===========================================================================*
 * Arch Engine - "Testers/core_input_context_test.cpp"                       *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"

 // Check if this tester is active
#if defined(ARCH_ENGINE_ECS_ECS_TEST)

#include "../Core/engine.hpp"
#include "../OS/inputManager.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <sstream>
#include <string>


using namespace Core;
using namespace OS;
using namespace Utils;


void test1(); // Tests File Watcher
void startLoggingService();


int main(int argc, char* argv[]) {
	try {
		startLoggingService();

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Started tests");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing filewatcher");
		test1();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished first test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Finished tests");

		return EXIT_SUCCESS;
	}
	catch (...) {
		return EXIT_FAILURE;
	}
}


void test1() {
	std::stringstream ss;

	if (Engine::getInstance().initialize("../../ArchEngine/Testers/"
		"core_engine_test_engine_config.lua")) {

		EventListener listener;
		listener.bind<Engine, &Engine::handleEvents>(&Engine::getInstance());
		EventManager::getInstance().addListener(listener, EVENT_CORE_QUIT);
		Engine::getInstance().watchFile("../../ArchEngine/Testers/"
			"windowHotReload.lua");

		Engine::getInstance().run();
	}
	else
		ServiceLocator::getFileLogger()->log<LOG_ERROR>(
			"Failed to initialize ArchEngine");

	Engine::getInstance().exit();

	ServiceLocator::getFileLogger()->log<LOG_INFO>(ss);
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


#endif	// ARCH_ENGINE_ECS_ECS_TEST