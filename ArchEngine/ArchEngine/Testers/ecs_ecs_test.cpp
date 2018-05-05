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
#include "../ECS/idGenerator.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <sstream>
#include <string>


using namespace Core;
using namespace ECS;
using namespace Utils;


void test1(); // Tests ECS id generator
void startLoggingService();


int main(int argc, char* argv[]) {
	try {
		startLoggingService();

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Started tests");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing IdGenerator");
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

	ss << "\nIdGenerator<IEntity> ids:\n";
	ss << "    int, int:   "
		<< IdGenerator<IEntity>::generateId<int>() << "\n";
	ss << "    int, int:   "
		<< IdGenerator<IEntity>::generateId<int>() << "\n";
	ss << "    int, float: "
		<< IdGenerator<IEntity>::generateId<float>() << "\n";
	ss << "    int, float: "
		<< IdGenerator<IEntity>::generateId<float>() << "\n";
	ss << "    int, float: "
		<< IdGenerator<IEntity>::generateId<float>() << "\n";
	ss << "    int, int:   "
		<< IdGenerator<IEntity>::generateId<int>() << "\n";
	ss << "    int, bool:  "
		<< IdGenerator<IEntity>::generateId<bool>() << "\n";

	ss << "IdGenerator<IComponent> ids:\n";
	ss << "    float, int:   "
		<< IdGenerator<IComponent>::generateId<int>() << "\n";
	ss << "    float, int:   "
		<< IdGenerator<IComponent>::generateId<int>() << "\n";
	ss << "    float, float: "
		<< IdGenerator<IComponent>::generateId<float>() << "\n";
	ss << "    float, float: "
		<< IdGenerator<IComponent>::generateId<float>() << "\n";
	ss << "    float, float: "
		<< IdGenerator<IComponent>::generateId<float>() << "\n";
	ss << "    float, int:   "
		<< IdGenerator<IComponent>::generateId<int>() << "\n";
	ss << "    float, bool:  "
		<< IdGenerator<IComponent>::generateId<bool>() << "\n";

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