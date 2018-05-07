/*===========================================================================*
 * Arch Engine - "OS/window.hpp"                                             *
 *                                                                           *
 * This class defines the attributes of the game window.                     *
 * Using SDL2 library for Operating System abstraction.                      *
 * - (https://www.libsdl.org/index.php).                                     *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 10/04/2018                                                       *
 * Last Modified: 07/05/2018                                                 *
 *===========================================================================*/


#ifndef OS_WINDOW_HPP
#define OS_WINDOW_HPP


#include "../Core/eventManager.hpp"
#include "../Config/engineMacros.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/fileWatcher.hpp"
#include "../Utils/serviceLocator.hpp"

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <cassert>
#include <string>


namespace OS {
	class Window {
	public:
		Window(bool vsync = true,
			bool anti_aliasing = true,
			bool fullscreen = true);

		~Window();

		// Memory related stuff is initialized and destroyed mannualy (and not
		// on the constructor/destructor), due to the fact that if we have
		// multiple windows inside a container ~Window() might be called,
		// invalidating some OpenGL context stuff, which is difficult to see.
		// The class has a flag (m_state) to ensure, through assertion, that
		// the user will remember to call the methods.
		bool initialize(
			const std::string& title,
			int pos_x = SDL_WINDOWPOS_CENTERED,
			int pos_y = SDL_WINDOWPOS_CENTERED,
			int width = 800,
			int height = 600,
			unsigned r_size = 8u,
			unsigned g_size = 8u,
			unsigned b_size = 8u,
			unsigned a_size = 8u,
			unsigned depth_size = 8u,
			unsigned stencil_size = 8u,
			unsigned ms_n_buffers = 1u,
			unsigned ms_n_samples = 4u,
			unsigned sdl_flags = SDL_WINDOW_OPENGL,
			unsigned context_major_version = 3,
			unsigned context_minor_version = 3);

		// Same as initialize, but loads parameters from file
		bool initializeFromConfigFile(const std::string& path);

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		void onFileModifiedEvent(Core::EventPtr e);
		void watchWindow(bool watch);
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

		void update();
		void destroy();

		void setPosition(int x, int y);

		void setVSync(bool state);
		void antiAliasing(unsigned n_samples);
		void fullscreen(bool state);

		void captureMouse();
		void releaseMouse();

		// Getters
		std::string getTitle() const;
		int getWidth() const;
		int getHeight() const;

		void getPosition(int* x, int* y) const;

		unsigned getMultisampleNBuffers() const;
		unsigned getMultisampleNSamples() const;

		// Read-only attributes, set once per window
		unsigned getRSize() const;
		unsigned getGSize() const;
		unsigned getBSize() const;
		unsigned getASize() const;

		unsigned getDepthSize() const;
		unsigned getStencilSize() const;

		// Window state
		bool isVSyncOn() const;
		bool isAntiAliasingOn() const;
		bool isFullscreen() const;

		// Setters
		void setSize(int width, int height);
		void setWidth(int width);
		void setHeight(int height);

	private:
		enum WindowState {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		// Flag to ensure resources have been freed
		WindowState m_state;

		// Window definition
		std::string m_title;
		int m_pos_x;
		int m_pos_y;
		int m_width;
		int m_height;
		
		// Anti-Aliasing attributes (ms stands for multisample)
		unsigned m_ms_n_buffers;
		unsigned m_ms_n_samples;

		// Number of bits of window color components
		unsigned m_r_size;
		unsigned m_g_size;
		unsigned m_b_size;
		unsigned m_a_size;

		// Number of bits of window buffer components
		unsigned m_depth_size;
		unsigned m_stencil_size;

		// Functionality flags
		bool m_vsync;
		unsigned m_anti_aliasing;
		bool m_fullscreen;

		// SDL variables
		SDL_Window* m_window;
		SDL_GLContext m_gl_context;

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		// Hot-reloading configuration
		std::string m_config_file_path;
		bool m_watch_file, m_file_being_watched;
		Core::EventListener m_file_modified_listener;
#endif	// ARCH_ENGINE_HOT_RELOAD_ON
	};
}


#endif	// OS_WINDOW_HPP