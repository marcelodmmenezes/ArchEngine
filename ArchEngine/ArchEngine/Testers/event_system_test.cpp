/*===========================================================================*
 * Arch Engine - "Testers/event_system_test.cpp"                             *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 30/04/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"

// Check if this tester is active
#if defined(ARCH_ENGINE_EVENT_SYSTEM_TEST)

#include "../Core/engine.hpp"
#include "../Core/concurrentEventQueue.hpp"
#include "../Utils/delegate.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>


using namespace Core;
using namespace Utils;


class Test1Event_1 : public IEvent {
public:
	Test1Event_1(const std::string& str) : m_str(str) {}

	std::string test() override {
		return "Test1Event_1";
	}

private:
	std::string m_str;
};

class Test1Event_2 : public IEvent {
public:
	Test1Event_2(const std::string& str) : m_str(str) {}

	std::string test() override {
		return "Test1Event_2";
	}

private:
	std::string m_str;
};

class Test2Aux {
public:
	void Method();
	void Method(const std::string& str);
	void Method(int num, const std::string& str);
};


void test1(); // Tests concurrent event queue
void test2(); // Tests delegate
void test2Aux_Function();
void test2Aux_Function(const std::string& str);
void test2Aux_Function(int num, const std::string& str);
void startLoggingService();


int main(int argc, char* argv[]) {
	try {
		startLoggingService();

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Started tests");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing concurrent event queue");
		test1();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished first test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing Delegate");
		test2();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished second test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Finished tests");

		return EXIT_SUCCESS;
	}
	catch (...) {
		return EXIT_FAILURE;
	}
}


void test1() {
	ConcurrentEventQueue queue("test1", 250u);

	if (queue.initialize()) {
		auto now = std::chrono::high_resolution_clock().now();

		for (int i = 0; i < 1000; i++) {
			std::shared_ptr<IEvent> evnt1(new Test1Event_1(std::to_string(i)));
			std::shared_ptr<IEvent> evnt2(new Test1Event_2(std::to_string(i)));

			queue.postEvent(std::move(evnt1));
			queue.postEvent(std::move(evnt2));
		}

		queue.destroy();

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Test1 elapsed time: " + std::to_string((
				std::chrono::high_resolution_clock().now() - now).count()));
	}
	else
		ServiceLocator::getFileLogger()->log<LOG_ERROR>(
			"Failed to initialize concurrent event queue");
}

void test2() {
	Delegate<void(const std::string&)> delegate[2];

	Test2Aux inst;

	std::vector<std::pair<Delegate<void(const std::string&)>, std::string>>
		delegates;


	delegate[0].bind<&test2Aux_Function>();
	delegates.push_back(
		std::make_pair(delegate[0], "\n\nTest2Aux_Function\n"));

	delegate[1].bind<Test2Aux, &Test2Aux::Method>(&inst);
	delegates.push_back(
		std::make_pair(delegate[1], "\n\nTest2Aux_Method\n"));

	for (auto& it : delegates)
		it.first.invoke(it.second);

	Delegate<void()> one_parameter_delegate;
	one_parameter_delegate.bind<&test2Aux_Function>();
	one_parameter_delegate.invoke();

	one_parameter_delegate.bind<Test2Aux, &Test2Aux::Method>(&inst);
	one_parameter_delegate.invoke();

	Delegate<void(int, const std::string&)> two_parameter_delegate;
	two_parameter_delegate.bind<&test2Aux_Function>();
	two_parameter_delegate.invoke(5, "Two parameters function delegate\n");

	two_parameter_delegate.bind<Test2Aux, &Test2Aux::Method>(&inst);
	two_parameter_delegate.invoke(5, "Two parameters method delegate\n");
}

void test2Aux_Function() {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"\n\nNo parameters function delegate\n");
}

void test2Aux_Function(const std::string& str) {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(str);
}

void test2Aux_Function(int num, const std::string& str) {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"\n\n" + std::to_string(num) + " " + str);
}

void Test2Aux::Method() {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"\n\nNo parameters method delegate\n");
}

void Test2Aux::Method(const std::string& str) {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(str);
}

void Test2Aux::Method(int num, const std::string& str) {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"\n\n" + std::to_string(num) + " " + str);
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


#endif	// ARCH_ENGINE_EVENT_SYSTEM_TEST