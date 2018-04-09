/*============================================================================*
 * Arch Engine - "Utils/serviceLocator.hpp"                                    *
 *                                                                            *
 * Service Locator - (Dependency injection)                                   *
 *                                                                            *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                      *
 * Created: 08/04/2018                                                        *
 * Last Modified: 09/04/2018                                                  *
 *============================================================================*/


#ifndef UTILS_SERVICE_LOCATOR_HPP
#define UTILS_SERVICE_LOCATOR_HPP


#include "logger.hpp"

#include <memory>


namespace Utils {
	// This class "provides global access to a service
	// without coupling the user to the concrete class
	// that implements it" (Robert Nystrom - Game Programming Patterns).
	class ServiceLocator {
	public:
		static Logger<ConsoleLogPolicy>* getConsoleLogger();
		static void provideConsoleLogger(std::shared_ptr
			<Logger<ConsoleLogPolicy>> console_logger);

		static Logger<FileLogPolicy>* getFileLogger();
		static void provideFileLogger(std::shared_ptr
			<Logger<FileLogPolicy>> file_logger);

	private:
		static std::shared_ptr<Logger<ConsoleLogPolicy>>
			m_console_logger;
		
		static std::shared_ptr<Logger<FileLogPolicy>>
			m_file_logger;
	};
}


#endif	// UTILS_SERVICE_LOCATOR_HPP