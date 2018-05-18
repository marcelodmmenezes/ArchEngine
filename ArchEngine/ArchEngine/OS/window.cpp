/*===========================================================================*
 * Arch Engine - "OS/window.hpp"                                             *
 *                                                                           *
 * This class defines the attributes of the game window.                     *
 * Using SDL2 library for Operating System abstraction.                      *
 * - (https://www.libsdl.org/index.php).                                     *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 10/04/2018                                                       *
 * Last Modified: 17/05/2018                                                 *
 *===========================================================================*/


#include "window.hpp"


using namespace Script;
using namespace Utils;


namespace OS {
	//------------------------------------------------ Constructor / Destructor
	Window::Window(bool vsync, bool anti_aliasing, bool fullscreen) :
		m_vsync(vsync), m_anti_aliasing(anti_aliasing),
		m_fullscreen(fullscreen), m_state(CONSTRUCTED),
		m_window(nullptr) {}

	Window::~Window() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == CONSTRUCTED || m_state == SAFE_TO_DESTROY);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			m_title + " window destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	//----------------------------------------------------------------- Methods
	bool Window::initialize(
		const std::string& title,
		int pos_x,
		int pos_y,
		int width,
		int height,
		unsigned r_size,
		unsigned g_size,
		unsigned b_size,
		unsigned a_size,
		unsigned depth_size,
		unsigned stencil_size,
		unsigned ms_n_buffers,
		unsigned ms_n_samples,
		unsigned sdl_flags,
		unsigned context_major_version,
		unsigned context_minor_version) {
		m_title = title;
		m_pos_x = pos_x;
		m_pos_y = pos_y;
		m_width = width;
		m_height = height;
		m_r_size = r_size;
		m_g_size = g_size;
		m_b_size = b_size;
		m_a_size = a_size;
		m_depth_size = depth_size;
		m_stencil_size = stencil_size;
		m_ms_n_buffers = ms_n_buffers;
		m_ms_n_samples = ms_n_samples;

		SDL_GL_LoadLibrary(nullptr);

		//--- SDL Window Attributes
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
			context_major_version);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
			context_minor_version);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, m_r_size);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, m_g_size);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, m_b_size);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, m_a_size);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_depth_size);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, m_stencil_size);

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,
			m_anti_aliasing ? "1" : "0");
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,
			m_anti_aliasing ? m_ms_n_buffers : 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,
			m_anti_aliasing ? m_ms_n_samples : 0);

		SDL_GL_SetSwapInterval(m_vsync ? SDL_TRUE : SDL_FALSE);
		//-------------------

		m_window = SDL_CreateWindow(m_title.c_str(),
			m_pos_x, m_pos_y, m_width, m_height,
			(m_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) | sdl_flags);

		if (!m_window) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Failed to initialize " + m_title);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		m_gl_context = SDL_GL_CreateContext(m_window);

		if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
			SDL_GL_DeleteContext(this->m_gl_context);
			SDL_DestroyWindow(this->m_window);
			m_window = nullptr;
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Failed to load OpenGL");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		m_state = INITIALIZED;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			m_title + " window initialized");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

		return true;
	}

	bool Window::initializeFromConfigFile(const std::string& path) {
		m_config_file_path = path;

		LuaScript lua_context;
		lua_context.initialize(path);

		// Maps sdl flags to their name string representation,
		// so we can load the flag by it's name from files.
		std::map<std::string, SDL_WindowFlags> sdl_window_flags_map = {
			std::make_pair("SDL_WINDOW_FULLSCREEN", SDL_WINDOW_FULLSCREEN),
			std::make_pair("SDL_WINDOW_OPENGL", SDL_WINDOW_OPENGL),
			std::make_pair("SDL_WINDOW_SHOWN", SDL_WINDOW_SHOWN),
			std::make_pair("SDL_WINDOW_HIDDEN", SDL_WINDOW_HIDDEN),
			std::make_pair("SDL_WINDOW_BORDERLESS", SDL_WINDOW_BORDERLESS),
			std::make_pair("SDL_WINDOW_RESIZABLE", SDL_WINDOW_RESIZABLE),
			std::make_pair("SDL_WINDOW_MINIMIZED", SDL_WINDOW_MINIMIZED),
			std::make_pair("SDL_WINDOW_MAXIMIZED", SDL_WINDOW_MAXIMIZED),
			std::make_pair("SDL_WINDOW_INPUT_GRABBED",
				SDL_WINDOW_INPUT_GRABBED),
			std::make_pair("SDL_WINDOW_INPUT_FOCUS", SDL_WINDOW_INPUT_FOCUS),
			std::make_pair("SDL_WINDOW_MOUSE_FOCUS", SDL_WINDOW_MOUSE_FOCUS),
			std::make_pair("SDL_WINDOW_FULLSCREEN_DESKTOP",
				SDL_WINDOW_FULLSCREEN_DESKTOP),
			std::make_pair("SDL_WINDOW_FOREIGN", SDL_WINDOW_FOREIGN),
			std::make_pair("SDL_WINDOW_ALLOW_HIGHDPI",
				SDL_WINDOW_ALLOW_HIGHDPI),
			std::make_pair("SDL_WINDOW_MOUSE_CAPTURE",
				SDL_WINDOW_MOUSE_CAPTURE)
#if defined(_MSC_VER) // The flags below are only available in X11
			,
			std::make_pair("SDL_WINDOW_ALWAYS_ON_TOP",
				SDL_WINDOW_ALWAYS_ON_TOP),
			std::make_pair("SDL_WINDOW_SKIP_TASKBAR", SDL_WINDOW_SKIP_TASKBAR),
			std::make_pair("SDL_WINDOW_UTILITY", SDL_WINDOW_UTILITY),
			std::make_pair("SDL_WINDOW_TOOLTIP", SDL_WINDOW_TOOLTIP),
			std::make_pair("SDL_WINDOW_POPUP_MENU", SDL_WINDOW_POPUP_MENU)
#endif
		};

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		m_file_being_watched = false;

		// Register for hot reload
		if (lua_context.get<bool>("hot_reload")) {
			m_watch_file = true;

			m_file_modified_listener.bind
				<Window, &Window::onFileModifiedEvent>(this);
			Core::EventManager::getInstance().addListener(
				m_file_modified_listener, Core::EVENT_FILE_MODIFIED);
		}
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

		m_vsync = lua_context.get<bool>("vsync");
		m_fullscreen = lua_context.get<bool>("fullscreen");

		int pos_x;
		if ((pos_x = lua_context.get<int>("pos_x")) == -1)
			pos_x = SDL_WINDOWPOS_CENTERED;

		int pos_y;
		if ((pos_y = lua_context.get<int>("pos_y")) == -1)
			pos_y = SDL_WINDOWPOS_CENTERED;

		auto flags = lua_context.getStringVector("sdl_flags");
		int sdl_flags = 0;

		for (auto& it : flags)
			sdl_flags |= sdl_window_flags_map[it];

		if (!initialize(
			lua_context.get<std::string>("title"),
			pos_x,
			pos_y,
			lua_context.get<int>("width"),
			lua_context.get<int>("height"),
			lua_context.get<int>("r_size"),
			lua_context.get<int>("g_size"),
			lua_context.get<int>("b_size"),
			lua_context.get<int>("a_size"),
			lua_context.get<int>("depth_size"),
			lua_context.get<int>("stencil_size"),
			lua_context.get<int>("ms_n_buffers"),
			lua_context.get<int>("ms_n_samples"),
			sdl_flags,
			lua_context.get<int>("context_major_version"),
			lua_context.get<int>("context_minor_version")))
			return false;

		lua_context.destroy();
		return true;
	}

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
	void Window::onFileModifiedEvent(Core::EventPtr e) {
		auto evnt = std::static_pointer_cast<FileModifiedEvent>(e);

		// See if the modified file was the window configuration
		if (evnt->getPath() != m_config_file_path)
			return;

		LuaScript lua_context;
		lua_context.initialize(m_config_file_path);

		setSize(lua_context.get<int>("width"),
			lua_context.get<int>("height"));

		int pos_x;
		if ((pos_x = lua_context.get<int>("pos_x")) == -1)
			pos_x = SDL_WINDOWPOS_CENTERED;

		int pos_y;
		if ((pos_y = lua_context.get<int>("pos_y")) == -1)
			pos_y = SDL_WINDOWPOS_CENTERED;

		setPosition(pos_x, pos_y);

		if (lua_context.get<bool>("fullscreen"))
			fullscreen(true);
		else
			fullscreen(false);

		m_watch_file = lua_context.get<bool>("hot_reload");

		lua_context.destroy();
	}

	void Window::watchWindow(bool watch) {
		m_watch_file = watch;
	}
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

	void Window::update() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		// Changes if the file is being watched according to parameters
		if (m_watch_file != m_file_being_watched) {
			Core::EventPtr evnt = std::make_shared<WatchFileEvent>(
				WatchFileEvent(m_config_file_path, m_watch_file));
			Core::EventManager::getInstance().sendEvent(evnt);
			m_file_being_watched = !m_file_being_watched;
		}
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

		SDL_GL_SwapWindow(this->m_window);
	}

	void Window::destroy() {
		if (m_state != INITIALIZED)
			return;

		if (m_window) {
			SDL_GL_DeleteContext(this->m_gl_context);
			SDL_DestroyWindow(this->m_window);
			m_window = nullptr;
		}

		Core::EventManager::getInstance().removeListener(
			m_file_modified_listener, Core::EVENT_FILE_MODIFIED);

		m_state = SAFE_TO_DESTROY;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			m_title + " window destroyed");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	void Window::setPosition(int x, int y) {
		m_pos_x = x;
		m_pos_y = y;

		if (m_state == INITIALIZED)
			SDL_SetWindowPosition(m_window, x, y);
	}

	void Window::setVSync(bool state) {
		m_vsync = state;

		if (m_state == INITIALIZED)
			SDL_GL_SetSwapInterval(state ? SDL_TRUE : SDL_FALSE);
	}

	void Window::antiAliasing(unsigned n_samples) {
		m_ms_n_samples = n_samples;
		m_anti_aliasing = m_ms_n_samples > 0 ? true : false;

		if (m_state == INITIALIZED) {
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,
				m_anti_aliasing ? "1" : "0");
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, m_ms_n_buffers);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, m_ms_n_samples);
		}
	}

	void Window::fullscreen(bool state) {
		m_fullscreen = state;

		if (m_state == INITIALIZED)
			SDL_SetWindowFullscreen(this->m_window,
				m_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}

	void Window::captureMouse() {
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	void Window::releaseMouse() {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	//----------------------------------------------------------------- Getters
	std::string Window::getTitle() const { return m_title; }
	int Window::getWidth() const { return m_width; }
	int Window::getHeight() const { return m_height; }
	
	void Window::getPosition(int* x, int* y) const {
		SDL_GetWindowPosition(m_window, x, y);
	}

	unsigned Window::getMultisampleNBuffers() const { return m_ms_n_buffers; }
	unsigned Window::getMultisampleNSamples() const { return m_ms_n_samples; }

	unsigned Window::getRSize() const { return m_r_size; }
	unsigned Window::getGSize() const { return m_g_size; }
	unsigned Window::getBSize() const { return m_b_size; }
	unsigned Window::getASize() const { return m_a_size; }

	unsigned Window::getDepthSize() const { return m_depth_size; }
	unsigned Window::getStencilSize() const { return m_stencil_size; }

	bool Window::isVSyncOn() const { return m_vsync; }
	bool Window::isAntiAliasingOn() const { return m_anti_aliasing; }
	bool Window::isFullscreen() const { return m_fullscreen; }

	//----------------------------------------------------------------- Setters
	void Window::setSize(int width, int height) {
		m_width = width;
		m_height = height;

		if (m_fullscreen)
			SDL_SetWindowFullscreen(m_window, 0);

		SDL_SetWindowSize(m_window, m_width, m_height);
	}

	void Window::setWidth(int width) {
		m_width = width;

		SDL_GetWindowSize(m_window, nullptr, &m_height);

		if (m_fullscreen)
			SDL_SetWindowFullscreen(m_window, 0);

		SDL_SetWindowSize(m_window, m_width, m_height);
	}

	void Window::setHeight(int height) {
		m_height = height;

		SDL_GetWindowSize(m_window, &m_width, nullptr);

		if (m_fullscreen)
			SDL_SetWindowFullscreen(m_window, 0);

		SDL_SetWindowSize(m_window, m_width, m_height);
	//-----------------------------------------------------------------------//
	}
}