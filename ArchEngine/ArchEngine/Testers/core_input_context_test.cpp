/*===========================================================================*
 * Arch Engine - "Testers/core_input_context_test.cpp"                       *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 19/04/2018                                                       *
 * Last Modified: 19/04/2018                                                 *
 *===========================================================================*/


#include "../Core/engineMacros.hpp"

 // Check if this tester is active
#if defined(ARCH_ENGINE_CORE_INPUT_CONTEXT_TEST)

#include "../Utils/serviceLocator.hpp"
#include "../Script/luaScript.hpp"


using namespace Script;
using namespace Utils;


void test1(); // Tests if context tables are read correctly
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