/*===========================================================================*
 * Arch Engine - "Core/window.hpp"                                           *
 *                                                                           *
 * This class defines the attributes of the game window.                     *
 * Using SDL2 library for Operating System abstraction.                      *
 * (https://www.libsdl.org/download-2.0.php).                                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 10/04/2018                                                       *
 * Last Modified: 10/04/2018                                                 *
 *===========================================================================*/


#include "window.hpp"


using namespace Utils;


namespace Core {
	//------------------------------------------------ Constructor / Destructor
	Window::Window(bool vsync, bool anti_aliasing, bool fullscreen) :
		m_vsync(vsync), m_anti_aliasing(anti_aliasing),
		m_fullscreen(fullscreen), m_state(CONSTRUCTED),
		m_window(nullptr) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			m_title + " window constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	}

	Window::~Window() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == CONSTRUCTED || m_state == SAFE_TO_DESTROY);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			m_title + " window destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
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
			m_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0 | sdl_flags);

		if (!m_window) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Failed to initialize " + m_title);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		}

		m_gl_context = SDL_GL_CreateContext(m_window);

		m_state = INITIALIZED;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			m_title + " window initialized");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	}

	void Window::update() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		SDL_GL_SwapWindow(this->m_window);
	}

	void Window::destroy() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		if (m_window) {
			SDL_GL_DeleteContext(this->m_gl_context);
			SDL_DestroyWindow(this->m_window);
			m_window = nullptr;
		}

		m_state = SAFE_TO_DESTROY;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			m_title + " window destroyed");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO
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
			SDL_GL_SetSwapInterval(state);
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
		this->m_width = width;
		this->m_height = height;

		if (this->m_fullscreen)
			SDL_SetWindowFullscreen(this->m_window, 0);

		SDL_SetWindowSize(this->m_window, this->m_width, this->m_height);
	}

	void Window::setWidth(int width) {
		this->m_width = width;

		int discard;

		SDL_GetWindowSize(this->m_window, &discard, &this->m_height);

		if (this->m_fullscreen)
			SDL_SetWindowFullscreen(this->m_window, 0);

		SDL_SetWindowSize(this->m_window, this->m_width, this->m_height);
	}

	void Window::setHeight(int height) {
		this->m_height = height;

		int discard;

		SDL_GetWindowSize(this->m_window, &this->m_width, &discard);

		if (this->m_fullscreen)
			SDL_SetWindowFullscreen(this->m_window, 0);

		SDL_SetWindowSize(this->m_window, this->m_width, this->m_height);
	//-----------------------------------------------------------------------//
	}
}