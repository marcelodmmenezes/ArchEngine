/*===========================================================================*
 * Arch Engine - "Graphics/framebuffer.cpp"                                  *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/05/2018                                                       *
 * Last Modified: 25/05/2018                                                 *
 *===========================================================================*/


#include "framebuffer.hpp"


using namespace Utils;


namespace Graphics {
	Framebuffer::Framebuffer() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Framebuffer constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	Framebuffer::~Framebuffer() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Framebuffer destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	bool Framebuffer::initialize(int width, int height) {
		m_width = width;
		m_height = height;

		//--------------------------------------------------------- Framebuffer
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		//------------------------------------------------------------- Texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		// Attaching texture to currently bound framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, m_texture, 0);

		//-------------------------------------------------------- Renderbuffer
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// Attaching renderbuffer to currently bound framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		//---------------------------------------------------------- Validation
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
			!= GL_FRAMEBUFFER_COMPLETE) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Failed to create framebuffer");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_state = INITIALIZED;

		return true;
	}

	void Framebuffer::destroy() {
		if (glIsTexture(m_texture))
			glDeleteTextures(1, &m_texture);

		if (glIsRenderbuffer(m_rbo))
			glDeleteRenderbuffers(1, &m_rbo);

		if (glIsFramebuffer(m_fbo))
			glDeleteFramebuffers(1, &m_fbo);

		m_state = SAFE_TO_DESTROY;
	}

	void Framebuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void Framebuffer::defaultFramebuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}