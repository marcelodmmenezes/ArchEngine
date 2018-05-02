/*===========================================================================*
 * Arch Engine - "Testers/event_system_test.cpp"                             *
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
#if defined(ARCH_ENGINE_EVENT_SYSTEM_TEST)

#include "../Core/engine.hpp"
#include "../Core/concurrentEventQueue.hpp"
#include "../Core/eventManager.hpp"
#include "../Utils/delegate.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>


using namespace Core;
using namespace Utils;


class Test1Aux {
public:
	void Method();
	void Method(const std::string& str);
	void Method(int num, const std::string& str);
};

class Test2Event_1 : public IEvent {
public:
	Test2Event_1(EventType type) : IEvent(type) {}
	~Test2Event_1() override {}

	EventType getType() override {
		return m_type;
	}
};

class Test2Event_2 : public IEvent {
public:
	Test2Event_2(EventType type) : IEvent(type) {}
	~Test2Event_2() override {}

	EventType getType() override {
		return m_type;
	}
};

class Test4AuxClass {
public:
	void method(EventPtr evnt) {
		ServiceLocator::getConsoleLogger()->log<LOG_INFO>(
			std::to_string(evnt->getType()));
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			std::to_string(evnt->getType()));
	}
};


void test1(); // Tests delegate
void test1Aux_Function();
void test1Aux_Function(const std::string& str);
void test1Aux_Function(int num, const std::string& str);
void test2(); // Tests concurrent event queue
void test3(); // Tests concurrent event queue from multiple threads
void test3Aux_Function(ConcurrentEventQueue* queue, unsigned i);
void test4(); // Tests the EventManager
void startLoggingService();


int main(int argc, char* argv[]) {
	try {
		startLoggingService();

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Started tests");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing delegate");
		test1();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished first test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing concurrent event queue");
		test2();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished second test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing concurrent event queue from multiple threads");
		test3();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished third test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing EventManager");
		test4();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished fourth test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Finished tests");

		return EXIT_SUCCESS;
	}
	catch (...) {
		return EXIT_FAILURE;
	}
}


void test1() {
	Delegate<void(const std::string&)> delegates[2];

	Test1Aux inst;

	std::vector<std::pair<Delegate<void(const std::string&)>, std::string>>
		delegate_vector;


	delegates[0].bind<&test1Aux_Function>();
	delegate_vector.push_back(
		std::make_pair(delegates[0], "\n\nTest2Aux_Function\n"));

	delegates[1].bind<Test1Aux, &Test1Aux::Method>(&inst);
	delegate_vector.push_back(
		std::make_pair(delegates[1], "\n\nTest2Aux_Method\n"));

	for (auto& it : delegate_vector)
		it.first.invoke(it.second);

	Delegate<void()> one_parameter_delegate;
	one_parameter_delegate.bind<&test1Aux_Function>();
	one_parameter_delegate.invoke();

	one_parameter_delegate.bind<Test1Aux, &Test1Aux::Method>(&inst);
	one_parameter_delegate.invoke();

	Delegate<void(int, const std::string&)> two_parameter_delegate;
	two_parameter_delegate.bind<&test1Aux_Function>();
	two_parameter_delegate.invoke(5, "Two parameters function delegate\n");

	two_parameter_delegate.bind<Test1Aux, &Test1Aux::Method>(&inst);
	two_parameter_delegate.invoke(5, "Two parameters method delegate\n");
}

void test1Aux_Function() {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"\n\nNo parameters function delegate\n");
}

void test1Aux_Function(const std::string& str) {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(str);
}

void test1Aux_Function(int num, const std::string& str) {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"\n\n" + std::to_string(num) + " " + str);
}

void Test1Aux::Method() {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"\n\nNo parameters method delegate\n");
}

void Test1Aux::Method(const std::string& str) {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(str);
}

void Test1Aux::Method(int num, const std::string& str) {
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"\n\n" + std::to_string(num) + " " + str);
}

void test2() {
	ConcurrentEventQueue queue;

	if (queue.initialize("test2", 250u)) {
		auto now = std::chrono::high_resolution_clock().now();

		for (int i = 0; i < 1000; i++) {
			std::shared_ptr<IEvent> evnt1(new Test2Event_1(TEST_EVENT_1));
			std::shared_ptr<IEvent> evnt2(new Test2Event_2(TEST_EVENT_2));

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

void test3() {
	ConcurrentEventQueue queue;

	if (queue.initialize("test3", 250u)) {
		auto now = std::chrono::high_resolution_clock().now();

		std::vector<std::thread*> threads;

		for (unsigned i = 0; i < 5; i++)
			threads.push_back(new std::thread(test3Aux_Function, &queue, i));

		for (auto& it : threads) {
			it->join();
			delete it;
		}

		queue.destroy();

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Test3 elapsed time: " + std::to_string((
				std::chrono::high_resolution_clock().now() - now).count()));
	}
	else
		ServiceLocator::getFileLogger()->log<LOG_ERROR>(
			"Failed to initialize concurrent event queue");
}

void test3Aux_Function(ConcurrentEventQueue* queue, unsigned i) {
	std::this_thread::sleep_for(std::chrono::duration<unsigned>(2));
	std::shared_ptr<IEvent> evnt1(new Test2Event_1(TEST_EVENT_3));
	queue->postEvent(std::move(evnt1));
}

void test4() {
	EventManager::getInstance().initialize(
		"../../ArchEngine/Testers/eventConfig.lua");

	Test4AuxClass inst;
	Delegate<void(EventPtr)> dlgt;
	dlgt.bind<Test4AuxClass, &Test4AuxClass::method>(&inst);

	std::shared_ptr<IEvent> evnt1(new Test2Event_1(TEST_EVENT_1));

	EventManager::getInstance().addListener(dlgt, evnt1->getType());
	EventManager::getInstance().addListener(dlgt, evnt1->getType());

	EventManager::getInstance().dispatch();

	EventManager::getInstance().removeListener(dlgt, evnt1->getType());

	EventManager::getInstance().destroy();
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