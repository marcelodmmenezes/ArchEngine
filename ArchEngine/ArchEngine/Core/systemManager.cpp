/*===========================================================================*
 * Arch Engine - "Core/systemManager.hpp"                                    *
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
 * Last Modified: 26/04/2018                                                 *
 *===========================================================================*/


#include "systemManager.hpp"


using namespace Script;
using namespace Utils;


namespace Core {
	SystemManager::SystemManager() : m_initialized(false) {}

	SystemManager::~SystemManager() {
		m_window.destroy();
	}

	SystemManager& SystemManager::getInstance() {
		static SystemManager instance;
		return instance;
	}

	bool SystemManager::initialize(const std::string& config_path) {
		//------------------------------------------------- Initializing Logger
		if (!m_initialized)
			startLoggingServices();
		else { // Prevents the engine to be initalized more than once
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
				"Could not initialize SDL2");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		//------------------------------------- Engine successfully initialized
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Engine successfully initialized");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		m_initialized = true;
		return true;
	}

	void SystemManager::run() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Running main loop");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		
		bool running = true;
		
		m_timer.init();

		int next_game_tick = m_timer.getCurrentTicks();
		// Game update loop counter
		int update_loops;
		// Value for rendering calculations
		float interpolation; 
		
		while (running) {
			//---------------------------------------------------- Time control
			m_timer.calc();

			//------------------------------------------------- Input gathering
			InputManager::getInstance().update(running);

			//----------------------------------------------- Game logic update
			update_loops = 0;

			while (m_timer.getCurrentTicks() > next_game_tick &&
				update_loops < m_max_frameskip) {
				// TODO: update the game

				next_game_tick += m_skip_ticks;
				update_loops++;
			}

			//------------------------------------------------- Scene rendering
			interpolation = (float)(m_timer.getCurrentTicks() + m_skip_ticks -
				next_game_tick) / (float)m_skip_ticks;

			// TODO: render scene

			m_window.update();
		}
	}

	void SystemManager::exit() {
		SDL_Quit();
		m_initialized = false;
	}

	bool SystemManager::isInitialized() const { return m_initialized; }

	void SystemManager::startLoggingServices() {
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

	bool SystemManager::loadConfigurations(const std::string& config_path) {
		LuaScript lua_context;
		lua_context.initialize(config_path);

		//------------------------------------------- Engine configuration file
		LuaScript engine_config;
		engine_config.initialize(lua_context.get<std::string>("engineConfig"));

		m_ticks_per_second = engine_config.get<int>("ticks_per_second");
		m_skip_ticks = engine_config.get<int>("skip_ticks");
		m_max_frameskip = engine_config.get<int>("max_frameskip");

		engine_config.destroy();

		//------------------------------------ Input systems configuration file
		if (!InputManager::getInstance().initialize(
			lua_context.get<std::string>("inputContexts")))
			return false;

		//------------------------------------------- Window configuration file
		if (!m_window.initializeFromConfigFile(
			lua_context.get<std::string>("windowConfig")))
			return false;

		lua_context.destroy();

		return true;
	}
}