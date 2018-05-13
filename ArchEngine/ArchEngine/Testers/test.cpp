/*===========================================================================*
 * Arch Engine - "Testers/test.cpp"                                          *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 12/05/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"

#if defined(ARCH_ENGINE_TEST)

#include "../Core/engine.hpp"
#include "../OS/inputManager.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <sstream>
#include <string>


using namespace Core;
using namespace OS;
using namespace Utils;


enum GameInputActions {
	TEST_ACTION,
	QUIT_ACTION = 2
};

enum GameInputStates {
	TEST_STATE
};


void test1Aux_Function1(EventPtr e);
void test1Aux_Function2(EventPtr e);
void test1Aux_Function3(EventPtr e);


int main(int argc, char* argv[]) {
	std::stringstream ss;

	Engine::startLoggingServices();

	if (Engine::getInstance().initialize("../../ArchEngine/Testers/"
		"core_engine_test_engine_config.lua")) {

		EventListener listener;

		listener.bind<&test1Aux_Function1>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_ACTION);

		listener.bind<&test1Aux_Function2>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_STATE);

		listener.bind<&test1Aux_Function3>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_CONTEXT);

		InputManager::getInstance().pushContext("test1");

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
	case GameInputActions::TEST_ACTION:
		std::cout << "TEST ACTION TRIGGERED\n";
		break;
	case GameInputActions::QUIT_ACTION:
		EventPtr quit_event(new CoreQuitEvent());
		EventManager::getInstance().postEvent(quit_event);
		break;
	}
}

void test1Aux_Function2(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputStateEvent>(e);

	switch (evnt->getValue()) {
	case GameInputStates::TEST_STATE:
		std::cout << "TEST STATE TRIGGERED\n";
		break;
	}
}

void test1Aux_Function3(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputContextEvent>(e);

	if (evnt->getState()) {
		// Context pushed
		std::cout << "CONTEXT PUSHED: " << evnt->getName() << std::endl;
	}
	else {
		// Context popped
		std::cout << "CONTEXT POPPED: " << evnt->getName() << std::endl;
	}
}


#endif	// ARCH_ENGINE_TEST