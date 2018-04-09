/*============================================================================*
 * Arch Engine - "Core/serviceLocator.hpp"                                    *
 *                                                                            *
 * Service Locator - (Dependency injection)                                   *
 *                                                                            *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                      *
 * Created: 08/04/2018                                                        *
 * Last Modified: 08/04/2018                                                  *
 *============================================================================*/


#include "serviceLocator.hpp"


namespace Core {
	// Static variables
	std::shared_ptr<Logger<ConsoleLogPolicy>>
		ServiceLocator::m_console_logger = nullptr;

	std::shared_ptr<Logger<FileLogPolicy>>
		ServiceLocator::m_file_logger = nullptr;

	// Methods
	Logger<ConsoleLogPolicy>* ServiceLocator::getConsoleLogger() {
		return m_console_logger.get();
	}

	void ServiceLocator::provideConsoleLogger(std::shared_ptr
		<Logger<ConsoleLogPolicy>> console_logger) {
		m_console_logger = console_logger;
	}

	Logger<FileLogPolicy>* ServiceLocator::getFileLogger() {
		return m_file_logger.get();
	}

	void ServiceLocator::provideFileLogger(std::shared_ptr
		<Logger<FileLogPolicy>> file_logger) {
		m_file_logger = file_logger;
	}
}