/*===========================================================================*
 * Arch Engine - "Core/window.hpp"                                           *
 *                                                                           *
 * This class defines the attributes of the game window.                     *
 * Using SDL2 library for Operating System abstraction.                      *
 * (https://www.libsdl.org/index.php).                                       *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 10/04/2018                                                       *
 * Last Modified: 10/04/2018                                                 *
 *===========================================================================*/


#ifndef CORE_WINDOW_HPP
#define CORE_WINDOW_HPP


#include "engineMacros.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cassert>
#include <string>


namespace Core {
	class Window {
	public:
		Window();
		~Window();

		// Memory related stuff is initialized and destroyed mannualy (and not
		// on the constructor/destructor), due to the fact that if we have
		// multiple windows inside a container the destructor might invalidate
		// some OpenGL context stuff, which is difficult to see.
		// The class has a flag (m_safe_to_destroy) to ensure, through
		// assertion, that the user will remember to call the methods.
		bool create(
			const std::string& title,
			unsigned width = 800u,
			unsigned height = 600u,
			unsigned r_size = 8u,
			unsigned g_size = 8u,
			unsigned b_size = 8u,
			unsigned a_size = 8u,
			unsigned depth_size = 8u,
			unsigned stencil_size = 8u,
			unsigned ms_n_buffers = 1u,
			unsigned ms_n_samples = 4u);

		void destroy();

		// Window functionalities toggle
		void setVSync(bool state);
		void antiAliasing(bool state);
		void fullscreen(bool state);

		// Getters
		std::string getTitle() const;
		unsigned getWidth() const;
		unsigned getHeight() const;

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

	private:
		// Window definition
		std::string m_title;
		unsigned m_width;
		unsigned m_height;

		// Anti-Aliasing attributes
		unsigned m_multisample_n_buffers;
		unsigned m_multisample_n_samples;

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
		bool m_anti_aliasing;
		bool m_fullscreen;

		// Flag to ensure resources have been freed
		bool m_safe_to_destroy;
	};
}


#endif	// CORE_WINDOW_HPP