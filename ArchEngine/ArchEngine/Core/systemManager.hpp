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
#include "../Utils/serviceLocator.hpp"

#include <cassert>


namespace Core {
	class SystemManager {
	public:
		~SystemManager();

		SystemManager(const SystemManager&) = delete;
		void operator=(const SystemManager&) = delete;

		static SystemManager& getInstance();

		bool initialize();
		void run();
		int exit();

		bool isInitialized() const;

	private:
		SystemManager();

		bool m_initialized;
	};
}


#endif	// CORE_SYSTEM_MANAGER_HPP