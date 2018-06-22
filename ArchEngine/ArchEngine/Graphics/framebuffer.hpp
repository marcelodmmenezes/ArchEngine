/*===========================================================================*
 * Arch Engine - "Graphics/framebuffer.hpp"                                  *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/05/2018                                                       *
 * Last Modified: 21/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_FRAMEBUFFER_HPP
#define GRAPHICS_FRAMEBUFFER_HPP


#include "../Config/engineMacros.hpp"
#include "../Core/eventManager.hpp"
#include "glad_3_3_core.hpp"
#include "../Utils/serviceLocator.hpp"
#include "../Utils/debugging.hpp"

#include <cassert>


namespace Graphics {
	enum FramebufferType {
		FB_COLOR_BUFFER,
		FB_DEPTH_MAP,
		FB_DEPTH_CUBE_MAP
	};

	class Framebuffer {
	public:
		Framebuffer();
		~Framebuffer();

		bool initialize(FramebufferType type, int width, int height);
		void destroy();

		void bind();
		unsigned getTextureId() const;
		static void defaultFramebuffer();

	private:
		bool initializeColorBuffer();
		bool initializeDepthMap();
		bool initializeDepthCubeMap();
		void onWindowResizeEvent(Core::EventPtr e);

		FramebufferType m_type;

		Core::EventListener m_window_size_listener;

		int m_width;
		int m_height;

		// OpenGL handles
		unsigned m_fbo; // Framebuffer
		unsigned m_rbo; // Renderbuffer
		unsigned m_texture; // Texture
	};
}


#endif	// GRAPHICS_FRAMEBUFFER_HPP