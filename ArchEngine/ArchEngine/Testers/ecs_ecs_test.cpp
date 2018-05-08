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


enum GameActions {
	TEST_ACTION
};

enum GameStates {
	TEST_STATE
};


void test1(); // Tests File Watcher
void test1Aux_Function1(EventPtr e);
void test1Aux_Function2(EventPtr e);
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

		listener.bind<&test1Aux_Function1>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_ACTION);

		listener.bind<&test1Aux_Function2>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_STATE);

		Engine::getInstance().run();
	}
	else
		ServiceLocator::getFileLogger()->log<LOG_ERROR>(
			"Failed to initialize ArchEngine");

	Engine::getInstance().exit();

	ServiceLocator::getFileLogger()->log<LOG_INFO>(ss);
}

void test1Aux_Function1(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputActionEvent>(e);

	switch (evnt->getValue()) {
	case GameActions::TEST_ACTION:
		std::cout << "TEST ACTION TRIGGERED";
		break;
	}
}

void test1Aux_Function2(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputStateEvent>(e);

	switch (evnt->getValue()) {
	case GameStates::TEST_STATE:
		std::cout << "TEST STATE TRIGGERED";
		break;
	}
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