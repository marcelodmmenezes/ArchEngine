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


#include "engine.hpp"


using namespace Graphics;
using namespace GUI;
using namespace OS;
using namespace Physics;
using namespace Script;
using namespace Utils;


//-------------------------------------------------------------- Lua Engine API
int captureMouse(lua_State* lua) {
	int argc = lua_gettop(lua);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"Lua capturing mouse with " + std::to_string(argc) + " arguments");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
	assert(argc == 0);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

	Core::Engine::getInstance().captureMouse();

	// No values returned to Lua
	return 0;
}

int releaseMouse(lua_State* lua) {
	int argc = lua_gettop(lua);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"Lua releasing mouse with " + std::to_string(argc) + " arguments");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
	assert(argc == 0);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

	Core::Engine::getInstance().releaseMouse();

	// No values returned to Lua
	return 0;
}
//-----------------------------------------------------------------------------


namespace Core {
	//------------------------------------------------------------------ Engine
	Engine::Engine() : m_initialized(false), m_running(false) {}

	Engine::~Engine() {
		m_window.destroy();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"ArchEngine destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	Engine& Engine::getInstance() {
		static Engine instance;
		return instance;
	}

	bool Engine::initialize(const std::string& config_path) {
		//------------------------------------------------- Initializing Logger
		// Prevents the engine to be initalized more than once
		if (m_initialized)  {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Trying to initialize the engine again");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		//--------------------------------------------------- Initializing SDL2
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not initialize SDL2");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		//------------------- Initializing file configuration dependent systems
		if (!loadConfigurations(config_path)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not load configurations");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		//----------------------------------------- Registering event listeners
		m_core_quit_listener.bind<Engine, &Engine::handleEvents>(this);
		EventManager::getInstance().addListener(
			m_core_quit_listener, EVENT_CORE_QUIT);

		//------------------------------------- Engine successfully initialized
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Engine successfully initialized");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		m_initialized = true;

		return true;
	}

	//-----------------------------------------------------------------------//
	//----------------------------------------------------------- GAME LOOP -//
	//-----------------------------------------------------------------------//
	void Engine::run() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Running main loop");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		
		m_running = true;
		
		m_timer.init();

		int next_game_tick = m_timer.getCurrentTicks();
		// Game update loop counter
		//int update_loops;
		
		while (m_running) {
			//---------------------------------------------------- Time control
			m_timer.calc();

			//--------------------------------------------------- Hot reloading
#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
			m_file_watcher.update();
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

			//------------------------------------------------- Input gathering
			InputManager::getInstance().update(m_running);

			//----------------------------------------------- Game logic update
			// TODO: framerate independent game update

			/*update_loops = 0;

			while (m_timer.getCurrentTicks() > next_game_tick &&
				update_loops < m_max_frameskip) {
				PhysicsManager::getInstance().update(
					(float)m_timer.getDeltaTime());

				next_game_tick += m_skip_ticks;
				update_loops++;

				m_timer.calc();
			}*/

			PhysicsManager::getInstance().update(
				(float)m_timer.getDeltaTime());

			//------------------------------------------------- Scene rendering
			GraphicsManager::getInstance().update(
				(float)m_timer.getDeltaTime());

			//----------------------------------------------------- Loop ending
			EventPtr evnt = std::make_shared<LoopFinishedEvent>(
				LoopFinishedEvent(m_timer.getDeltaTime(),
					m_timer.getFrameRate()));
			EventManager::getInstance().sendEvent(evnt);

			//------------------------------------------------------ GUI update
			GUIManager::getInstance().update(
				(float)m_timer.getDeltaTime());

			m_window.update();
		}
	}

	void Engine::exit() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Destroying GUIManager");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		GUIManager::getInstance().destroy();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Destroying PhysicsManager");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		PhysicsManager::getInstance().destroy();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Destroying GraphicsManager");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		GraphicsManager::getInstance().destroy();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Destroying MaterialManager");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		MaterialManager::getInstance().destroy();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Quiting SDL");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		SDL_Quit();

		m_initialized = false;
	}

	void Engine::captureMouse() {
		m_window.captureMouse();
	}

	void Engine::releaseMouse() {
		m_window.releaseMouse();
	}

	void Engine::handleEvents(EventPtr evnt) {
		switch (evnt->getType()) {
		case EVENT_CORE_QUIT:
			m_running = false;
			break;
		}
	}

	bool Engine::isInitialized() const { return m_initialized; }
	
#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
	bool Engine::watchFile(const std::string& path) {
		return m_file_watcher.addFile(path);
	}

	bool Engine::unwatchFile(const std::string& path) {
		return m_file_watcher.removeFile(path);
	}
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

	void Engine::startLoggingServices() {
#if !defined(ARCH_ENGINE_LOGGER_SUPPRESS_INFO) || \
	!defined(ARCH_ENGINE_LOGGER_SUPPRESS_WARNING) || \
	!defined(ARCH_ENGINE_LOGGER_SUPPRESS_ERROR) || \
	!defined(ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG)

		std::shared_ptr<Logger<ConsoleLogPolicy>>
			console_logger(new Logger<ConsoleLogPolicy>(""));

		std::shared_ptr<Logger<FileLogPolicy>>
			file_logger(new Logger<FileLogPolicy>("log.txt"));

		console_logger->setThreadName("mainThread");
		file_logger->setThreadName("mainThread");

		ServiceLocator::provideConsoleLogger(console_logger);
		ServiceLocator::provideFileLogger(file_logger);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getConsoleLogger()->log<LOG_INFO>(
			"Logging Systems online...");
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Logging Systems online...");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

#endif	/* !defined(ARCH_ENGINE_LOGGER_SUPPRESS_INFO) || \
	!defined(ARCH_ENGINE_LOGGER_SUPPRESS_WARNING) || \
	!defined(ARCH_ENGINE_LOGGER_SUPPRESS_ERROR) || \
	!defined(ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG) */
	}

	bool Engine::loadConfigurations(const std::string& config_path) {
		LuaScript lua_context;
		lua_context.initialize(config_path);

		//------------------------------------------- Engine configuration file
		LuaScript engine_config;
		
		if (!engine_config.initialize(
			lua_context.get<std::string>("files.engineConfig")))
			return false;

		m_ticks_per_second = engine_config.get<int>("ticks_per_second");
		m_skip_ticks = engine_config.get<int>("skip_ticks");
		m_max_frameskip = engine_config.get<int>("max_frameskip");

		engine_config.destroy();

		//------------------------------------------- Window configuration file
		if (!m_window.initializeFromConfigFile(
			lua_context.get<std::string>("files.windowConfig"))) {
			lua_context.destroy();
			return false;
		}

		//---------------------------------- Graphics system configuration file
		if (!GraphicsManager::getInstance().initializeFromConfigFile(
			lua_context.get<std::string>("files.graphicsConfig"))) {
			lua_context.destroy();
			return false;
		}

		//----------------------------------- Physics system configuration file
		if (!PhysicsManager::getInstance().initializeFromConfigFile(
			lua_context.get<std::string>("files.physicsConfig"))) {
			lua_context.destroy();
			return false;
		}

		//--------------------------------------- GUI system configuration file
		if (!GUIManager::getInstance().initializeFromConfigFile(
			lua_context.get<std::string>("files.guiConfig"))) {
			lua_context.destroy();
			return false;
		}

		//------------------------------------ Input systems configuration file
		if (!InputManager::getInstance().initialize(
			lua_context.get<std::string>("files.inputContexts"))) {
			lua_context.destroy();
			return false;
		}

		//------------------------------------------ Lua API configuration file
		if (!EngineLuaAPI::getInstance().initialize(
			lua_context.get<std::string>("files.luaAPI"))) {
			lua_context.destroy();
			return false;
		}

		lua_context.destroy();

		return true;
	}
}