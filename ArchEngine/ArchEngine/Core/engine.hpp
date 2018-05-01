/*===========================================================================*
 * Arch Engine - "Core/engine.hpp"                                           *
 *                                                                           *
 * Heart of the engine. Responsible for systems intialization and shutdown,  *
 * the game loop and other stuff I'll add as the egine grows...              *
 *                                                                           *
 * Game loop based in:                                                       *
 * - (http://www.koonsolo.com/news/dewitters-gameloop/)                      *
 * - (https://gafferongames.com/post/fix_your_timestep/)                     *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/04/2018                                                       *
 * Last Modified: 30/04/2018                                                 *
 *===========================================================================*/


#ifndef CORE_ENGINE_HPP
#define CORE_ENGINE_HPP


#include "../Config/engineMacros.hpp"
#include "inputManager.hpp"
#include "window.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"
#include "../Utils/timer.hpp"

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <memory>
#include <string>


namespace Core {
	class Engine {
	public:
		~Engine();

		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		static Engine& getInstance();

		bool initialize(const std::string& config_path);
		void run();
		void exit();

		bool isInitialized() const;

	private:
		Engine();

		void startLoggingServices();
		bool loadConfigurations(const std::string& config_path);

		bool m_initialized;

		// For now there's only one window which is owned by the SystemManager
		Window m_window;

		// Timer
		Utils::Timer m_timer;

		// Game loop time control variables
		int m_ticks_per_second;
		int m_skip_ticks;
		int m_max_frameskip;
	};
}


#endif	// CORE_ENGINE_HPP