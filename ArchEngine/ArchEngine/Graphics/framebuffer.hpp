/*===========================================================================*
 * Arch Engine - "Graphics/framebuffer.hpp"                                  *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/05/2018                                                       *
 * Last Modified: 25/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_FRAMEBUFFER_HPP
#define GRAPHICS_FRAMEBUFFER_HPP


#include "../Config/engineMacros.hpp"
#include "glad_3_3_core.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cassert>


namespace Graphics {
	class Framebuffer {
	public:
		Framebuffer();
		~Framebuffer();

		bool initialize(int width, int height);
		void destroy();

		void bind();
		unsigned getTextureId() const;
		static void defaultFramebuffer();

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		State m_state;

		int m_width;
		int m_height;

		// OpenGL handle
		unsigned m_fbo; // Framebuffer
		unsigned m_rbo; // Renderbuffer
		unsigned m_texture; // Texture
	};
}


#endif	// GRAPHICS_FRAMEBUFFER_HPP