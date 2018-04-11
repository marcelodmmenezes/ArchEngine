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
	Window::Window() {
#ifndef ARCH_ENGINE_SUPRESS_INFO_LOGGING
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			m_title + " window instantiated");
#endif
	}

	Window::~Window() {
		assert(m_safe_to_destroy);

#ifndef ARCH_ENGINE_SUPRESS_INFO_LOGGING
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			m_title + " window destroyed");
#endif
	}

	bool Window::create(
		const std::string& title,
		unsigned width,
		unsigned height,
		unsigned r_size,
		unsigned g_size,
		unsigned b_size,
		unsigned a_size,
		unsigned depth_size,
		unsigned stencil_size,
		unsigned ms_n_buffers,
		unsigned ms_n_samples) {
		m_title = title;
		m_width = width;
		m_height = height;
		m_r_size = r_size;
		m_g_size = g_size;
		m_b_size = b_size;
		m_a_size = a_size;
		m_depth_size = depth_size;
		m_stencil_size = stencil_size;
		m_multisample_n_buffers = ms_n_buffers;
		m_multisample_n_samples = ms_n_samples;
		m_safe_to_destroy = false;
	}

	void Window::destroy() {
		m_safe_to_destroy = true;
	}

	// Window functionalities toggle
	void Window::setVSync(bool state) {
	}

	void Window::antiAliasing(bool state) {
	}

	void Window::fullscreen(bool state) {
	}

	// Getters
	std::string Window::getTitle() const {
		return m_title;
	}

	unsigned Window::getWidth() const {
		return m_width;
	}

	unsigned Window::getHeight() const {
		return m_height;
	}

	unsigned Window::getMultisampleNBuffers() const {
		return m_multisample_n_buffers;
	}

	unsigned Window::getMultisampleNSamples() const {
		return m_multisample_n_samples;
	}

	// Read-only attributes, set once per window
	unsigned Window::getRSize() const {
		return m_r_size;
	}

	unsigned Window::getGSize() const {
		return m_g_size;
	}
	
	unsigned Window::getBSize() const {
		return m_b_size;
	}
	
	unsigned Window::getASize() const {
		return m_a_size;
	}

	unsigned Window::getDepthSize() const {
		return m_depth_size;
	}
	
	unsigned Window::getStencilSize() const {
		return m_stencil_size;
	}

	// Window state
	bool Window::isVSyncOn() const {
		return m_vsync;
	}
	
	bool Window::isAntiAliasingOn() const {
		return m_anti_aliasing;
	}
	
	bool Window::isFullscreen() const {
		return m_fullscreen;
	}
}