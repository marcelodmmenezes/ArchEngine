/*===========================================================================*
 * Arch Engine - "Core/systemManager.hpp"                                    *
 *                                                                           *
 * Heart of the engine. Responsible for systems intialization and shutdown,  *
 * the game loop and other stuff I'll add as the egine grows...              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/04/2018                                                       *
 * Last Modified: 25/04/2018                                                 *
 *===========================================================================*/


#ifndef CORE_SYSTEM_MANAGER_HPP
#define CORE_SYSTEM_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "inputManager.hpp"
#include "window.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <memory>
#include <string>


namespace Core {
	class SystemManager {
	public:
		~SystemManager();

		SystemManager(const SystemManager&) = delete;
		void operator=(const SystemManager&) = delete;

		static SystemManager& getInstance();

		bool initialize(const std::string& config_path);
		void run();
		void exit();

		bool isInitialized() const;

	private:
		SystemManager();

		void startLoggingServices();
		bool loadConfigurations(const std::string& config_path);

		bool m_initialized;
	};
}


#endif	// CORE_SYSTEM_MANAGER_HPP