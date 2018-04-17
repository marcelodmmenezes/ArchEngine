/*===========================================================================*
 * Arch Engine - "Testers/core_window_test.cpp"                              *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 11/04/2018                                                       *
 * Last Modified: 16/04/2018                                                 *
 *===========================================================================*/


#include "../Core/engineMacros.hpp"

 // Check if this tester is active
#if defined(ARCH_ENGINE_CORE_WINDOW_TEST)

#include "../Core/window.hpp"

#include "../Utils/logger.hpp"
#include "../Utils/serviceLocator.hpp"

#include <chrono>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <string>
#include <thread>


using namespace Utils;


//--- Time control
std::chrono::time_point<std::chrono::high_resolution_clock> g_start, g_end;

//-- Logger controls to avoid writing same stuff over and over again
bool g_second_test_session_logs[] = {
	true, true, true, true, true, true, true, true
};


void startLoggingService();
void firstWindowTest();	// Tests window initialization and destruction
void secondWindowTest(); // Tests window settings
void thirdWindowTest(); // Tests multiple windows
void fourthWindowTest(); // Tests multiple thread windows


int main(int argc, char* argv[]) {
	try {
		startLoggingService(); 

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Started tests");

		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing window creation and destruction");
		firstWindowTest();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished first test\n\
----------------------------------------------------------------------------");

		// Waits one second to next test
		std::this_thread::sleep_for(std::chrono::duration<int>(1));
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing window settings");
		secondWindowTest();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished second test\n\
----------------------------------------------------------------------------");

		// Waits one second to next test
		std::this_thread::sleep_for(std::chrono::duration<int>(1));
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing multiple windows");
		thirdWindowTest();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished third test\n\
----------------------------------------------------------------------------");

		// Waits one second to next test
		std::this_thread::sleep_for(std::chrono::duration<int>(1));
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Testing multiple windows");
		fourthWindowTest();
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Finished third test\n\
----------------------------------------------------------------------------");

		ServiceLocator::getFileLogger()->log<LOG_INFO>("Finished tests");

		std::this_thread::sleep_for(std::chrono::duration<int>(2));
	}
	catch (...) {
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

void firstWindowTest() {
	Core::Window w1(true, true, false);

	w1.initialize("First window");
	ServiceLocator::getFileLogger()->log<LOG_DEBUG>("w1 initialized");

	g_start = std::chrono::high_resolution_clock::now();

	do {
		g_end = std::chrono::high_resolution_clock::now();
		w1.update();
	} while (g_end - g_start < std::chrono::duration<int>(5));

	w1.destroy();
	ServiceLocator::getFileLogger()->log<LOG_DEBUG>("w1 destroyed");
}

void secondWindowTest() {
	Core::Window w2(true, true, false);

	w2.initialize("Second window");
	ServiceLocator::getFileLogger()->log<LOG_DEBUG>("w2 initialized");

	g_start = std::chrono::high_resolution_clock::now();

	do {
		g_end = std::chrono::high_resolution_clock::now();
		w2.update();

		// Switches antialiasing and vsync states at marks 17 and 19
		if (g_end - g_start > std::chrono::duration<int>(19)) {
			if (g_second_test_session_logs[0]) {
				ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
					"Antialiasing and vsync on");
				g_second_test_session_logs[0] = false;
			}
			w2.antiAliasing(16);
			w2.setVSync(true);
		}
		else if (g_end - g_start > std::chrono::duration<int>(17)) {
			if (g_second_test_session_logs[1]) {
			ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
				"Antialiasing and vsync off");
				g_second_test_session_logs[1] = false;
			}
			w2.antiAliasing(0);
			w2.setVSync(false);
		}
		// Recenters window at mark 15
		else if (g_end - g_start > std::chrono::duration<int>(15)) {
			if (g_second_test_session_logs[2]) {
			ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
				"Window centered");
				g_second_test_session_logs[2] = false;
			}
			w2.setPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
		// Resizes at marks 8, 10, 12
		else if (g_end - g_start > std::chrono::duration<int>(12)) {
			if (g_second_test_session_logs[3]) {
			ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
				"Resized to 800, 600");
				g_second_test_session_logs[3] = false;
			}
			w2.setHeight(600);
		}
		else if (g_end - g_start > std::chrono::duration<int>(10)) {
			if (g_second_test_session_logs[4]) {
			ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
				"Resized to 800, 300");
				g_second_test_session_logs[4] = false;
			}
			w2.setWidth(800);
		}
		else if (g_end - g_start > std::chrono::duration<int>(8)) {
			if (g_second_test_session_logs[5]) {
			ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
				"Resized to 400, 300");
				g_second_test_session_logs[5] = false;
			}
			w2.setSize(400, 300);
		}
		// Set fullscreen at mark 5
		else if (g_end - g_start > std::chrono::duration<int>(5)) {
			if (g_second_test_session_logs[6]) {
			ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
				"Fullscreen on");
				g_second_test_session_logs[6] = false;
			}
			w2.fullscreen(true);
		}
		// Changes window position at mark 2
		else if (g_end - g_start > std::chrono::duration<int>(2)) {
			if (g_second_test_session_logs[7]) {
			ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
				"Changing position to 50, 50");
				g_second_test_session_logs[7] = false;
			}
			w2.setPosition(50, 50);
		}
	} while (g_end - g_start < std::chrono::duration<int>(20));

	w2.destroy();
	ServiceLocator::getFileLogger()->log<LOG_DEBUG>("w2 destroyed");
}

void thirdWindowTest() {
	Core::Window w3(true, true, false), w4(true, true, false);

	bool w4_init = false;

	w3.initialize("Third window");
	w3.setSize(400, 300);
	w3.setPosition(200, 50);
	ServiceLocator::getFileLogger()->log<LOG_DEBUG>("w3 initialized");

	g_start = std::chrono::high_resolution_clock::now();

	do {
		g_end = std::chrono::high_resolution_clock::now();

		// Initializes w4 at mark 5
		if (!w4_init && g_end - g_start > std::chrono::duration<int>(5)) {
			w4.initialize("Fourth window");
			w4.setSize(400, 300);
			w4.setPosition(1200, 50);
			ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
				"w4 initialized");
			w4_init = true;
		}
	} while (g_end - g_start < std::chrono::duration<int>(10));

	w3.destroy();
	ServiceLocator::getFileLogger()->log<LOG_DEBUG>("w3 destroyed");

	w4.destroy();
	ServiceLocator::getFileLogger()->log<LOG_DEBUG>("w4 destroyed");
}

void fourthWindowTest() {
	std::thread t1(firstWindowTest);
	std::thread t2(thirdWindowTest);
	t1.join();
	t2.join();
}


#endif	// ARCH_ENGINE_CORE_WINDOW_TEST