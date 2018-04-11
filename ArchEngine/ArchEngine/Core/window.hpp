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


#ifndef CORE_WINDOW_HPP
#define CORE_WINDOW_HPP


#include "../Utils/serviceLocator.hpp"

#include <string>


namespace Core {
	class Window {
	public:
		Window();
		~Window();

		// Memory related stuff is initialized and destroyed mannualy (and not
		// on the constructor/destructor), due to the fact that if we have
		// multiple windows inside a container the copy constructor might
		// invalidate some OpenGL context stuff.
		// The class has a flag (m_safe_to_destroy) to ensure the user will
		// remember to call the methods.
		bool create();
		void destroy();

		// Read-only attributes, set once per window
		unsigned getRSize() const;
		unsigned getGSize() const;
		unsigned getBSize() const;
		unsigned getASize() const;

		unsigned getDepthSize() const;
		unsigned getStencilSize() const;

		unsigned getMultisampleNBuffers() const;
		unsigned getMultisampleNSamples() const;

		// Window state
		bool isVSyncOn() const;
		bool isAntiAliasingOn() const;
		bool isFullscreen() const;

		// Window functionalities toggle
		void setVSync(bool state);
		void antiAliasing(bool state);
		void fullscreen(bool state);

	private:
		// Visual attributes
		std::string m_title;
		unsigned m_width;
		unsigned m_height;

		// Color attributes
		unsigned m_red_size;
		unsigned m_green_size;
		unsigned m_blue_size;
		unsigned m_alpha_size;

		// Buffer attributes
		unsigned m_depth_size;
		unsigned m_stencil_size;

		// Anti-Aliasing attributes
		unsigned m_multisample_n_buffers;
		unsigned m_multisample_n_samples;

		// Functionality flags
		bool m_vsync;
		bool m_anti_aliasing;
		bool m_fullscreen;

		// Flag to ensure resources have been freed
		bool m_safe_to_destroy;
	};
}


#endif	// CORE_WINDOW_HPP