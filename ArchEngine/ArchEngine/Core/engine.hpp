/*===========================================================================*
 * Arch Engine - "Core/engine.hpp"                                           *
 *                                                                           *
 * Heart of the engine. Responsible for systems intialization and shutdown,  *
 * the game loop and other stuff I'll add as the egine grows...              *
 *                                                                           *
 * Game loop based in:                                                       *
 * - (https://gafferongames.com/post/fix_your_timestep/)                     *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/04/2018                                                       *
 * Last Modified: 21/06/2018                                                 *
 *===========================================================================*/


#ifndef CORE_ENGINE_HPP
#define CORE_ENGINE_HPP


#include "event.hpp"
#include "../Config/engineMacros.hpp"
#include "../Graphics/graphicsManager.hpp"
#include "../GUI/guiManager.hpp"
#include "../OS/inputManager.hpp"
#include "../OS/window.hpp"
#include "../Physics/physicsManager.hpp"
#include "../Script/engineLuaAPI.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"
#include "../Utils/timer.hpp"

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
#include "../Utils/fileWatcher.hpp"
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <iomanip>
#include <memory>
#include <string>


 //------------------------
 //--------- Lua Engine API
 //------------------------
extern "C" {
	int captureMouse(lua_State* lua);
	int releaseMouse(lua_State* lua);
}
//-------------------------------------------------------------------------


namespace Core {
	class Engine {
	public:
		~Engine();

		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		static Engine& getInstance();

		static void startLoggingServices();

		bool initialize(const std::string& config_path);
		void run();
		void exit();

		void captureMouse();
		void releaseMouse();

		void handleEvents(EventPtr evnt);

		bool isInitialized() const;

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		bool watchFile(const std::string& path);
		bool unwatchFile(const std::string& path);
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

	private:
		Engine();

		bool loadConfigurations(const std::string& config_path);

		bool m_initialized;
		bool m_running;

		// For now there's only one window which is owned by the SystemManager
		OS::Window m_window;

		// Timer
		Utils::Timer m_timer;

		// Game loop time control variables
		int m_ticks_per_second;
		int m_skip_ticks;
		int m_max_frameskip;

		EventListener m_core_quit_listener;

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		// Hot reloading system
		Utils::FileWatcher m_file_watcher;
#endif	// ARCH_ENGINE_HOT_RELOAD_ON
	};
}


#endif	// CORE_ENGINE_HPP