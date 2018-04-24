/*===========================================================================*
 * Arch Engine - "Testers/core_input_context_test.cpp"                       *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 19/04/2018                                                       *
 * Last Modified: 24/04/2018                                                 *
 *===========================================================================*/


#include "../Core/engineMacros.hpp"

 // Check if this tester is active
#if defined(ARCH_ENGINE_CORE_INPUT_CONTEXT_TEST)

#include "../Core/inputManager.hpp"
#include "../Core/window.hpp"
#include "../Utils/serviceLocator.hpp"
#include "../Script/luaScript.hpp"


using namespace Core;
using namespace Script;
using namespace Utils;


void test1(); // Tests if context tables are read correctly
void test2(); // Tests if InputContext is parsing files correctly
void test3(); // Tests if InputManager is handling files correctly
void test4(); // Tests an input loop
void startLoggingService();


int main(int argc, char* argv[]) {
	try {
		startLoggingService();

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Started tests");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing if context tables are read correctly");
		test1();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished first test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing if InputContext is parsing files correctly");
		test2();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished second test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing if InputManager is handling files correctly");

		InputManager::getInstance().initialize(
			"../../ArchEngine/Config/inputContexts.lua");

		test3();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished third test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing an input loop");
		test4();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished fourth test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Finished tests");
	}
	catch (...) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


void test1() {
	std::stringstream ss;

	LuaScript script;
	script.initialize("../../ArchEngine/Config/testContext.lua");

	std::vector<std::pair<std::string, std::string>> mapping =
		script.getTablePairs("context");

	ss << "\n\n    Values of [context]:\n";
	for (auto &it : mapping)
		ss << it.first << ":" << it.second << ", ";
	ss << "\n\n";

	mapping = script.getTablePairs("context.actions");

	ss << "    Values of [context.actions]:\n";
	for (auto &it : mapping)
		ss << it.first << ":" << it.second << ", ";
	ss << "\n\n";

	mapping = script.getTablePairs("context.states");

	ss << "    Values of [context.states]:\n";
	for (auto &it : mapping)
		ss << it.first << ":" << it.second << ", ";
	ss << "\n\n";

	mapping = script.getTablePairs("context.ranges");

	ss << "    Values of [context.ranges]:\n";
	for (auto &it : mapping)
		ss << it.first << ":" << it.second << ", ";
	ss << "\n\n";

	ServiceLocator::getFileLogger()->log<LOG_INFO>(ss);

	script.destroy();
}

void test2() {
	InputContext ic("../../ArchEngine/Config/testContext.lua");
}

void test3() {
	InputManager::getInstance().contextOn("test");

	// Simulates some button presses
	InputManager::getInstance().setKeyState(SDLK_w, true, false);
	InputManager::getInstance().setModState(KMOD_LSHIFT, true, false);
	InputManager::getInstance().setAxisValue(MOUSE_AXIS_X, 200);
	InputManager::getInstance().setAxisValue(MOUSE_AXIS_Y, 100);

	bool discard;
	InputManager::getInstance().dispatch(discard);

	InputManager::getInstance().contextOff("test");
}

void test4() {
	Window window(true, true, false);
	window.initialize("InputLoop test");

	InputManager::getInstance().contextOn("test");

	bool running = true;

	while (running) {
		InputManager::getInstance().update();
		InputManager::getInstance().dispatch(running);
		InputManager::getInstance().clearInput();
	}

	window.destroy();
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


#endif	// ARCH_ENGINE_CORE_INPUT_CONTEXT_TEST