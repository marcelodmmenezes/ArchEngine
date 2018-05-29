/*===========================================================================*
 * Arch Engine - "Graphics/framebuffer.cpp"                                  *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/05/2018                                                       *
 * Last Modified: 29/05/2018                                                 *
 *===========================================================================*/


#include "framebuffer.hpp"


using namespace Core;
using namespace Utils;


namespace Graphics {
	Framebuffer::Framebuffer() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Framebuffer constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

		m_window_size_listener.bind
			<Framebuffer, &Framebuffer::onWindowResizeEvent>(this);
		EventManager::getInstance().addListener(
			m_window_size_listener, EVENT_WINDOW_RESIZE);
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

	bool Framebuffer::initialize(FramebufferType type, int width, int height) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS
		bool success = false;

		m_type = type;
		m_width = width;
		m_height = height;

		switch (type) {
		case FB_COLOR_BUFFER:
			success = initializeColorBuffer();
			break;
		case FB_DEPTH_MAP:
			success = initializeDepthMap();
			break;
		case FB_DEPTH_CUBE_MAP:
			success = initializeDepthCubeMap();
			break;
		}

		return success;
	}

	void Framebuffer::destroy() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

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

	unsigned Framebuffer::getTextureId() const {
		return m_texture;
	}

	void Framebuffer::defaultFramebuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool Framebuffer::initializeColorBuffer() {
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

		//--------------------------------------------------------- Framebuffer
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// Attaching renderbuffer to currently bound framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		//---------------------------------------------------------- Validation
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
			!= GL_FRAMEBUFFER_COMPLETE) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"\nFailed to create framebuffer\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_state = INITIALIZED;

		return true;
	}

	bool Framebuffer::initializeDepthMap() {
		//------------------------------------------------------------- Texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

		glBindTexture(GL_TEXTURE_2D, 0);

		//--------------------------------------------------------- Framebuffer
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// Attaching texture to currently bound framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, m_texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		//---------------------------------------------------------- Validation
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
			!= GL_FRAMEBUFFER_COMPLETE) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"\nFailed to create framebuffer\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_state = INITIALIZED;

		return true;
	}

	bool Framebuffer::initializeDepthCubeMap() {
		//------------------------------------------------------------- Texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

		// Cube texture
		for (unsigned i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				GL_DEPTH_COMPONENT, m_width, m_height, 0,
				GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		//--------------------------------------------------------- Framebuffer
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// Attaching texture to currently bound framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		//---------------------------------------------------------- Validation
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER)
			!= GL_FRAMEBUFFER_COMPLETE) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"\nFailed to create framebuffer\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_state = INITIALIZED;

		return true;
	}

	void Framebuffer::onWindowResizeEvent(Core::EventPtr e) {
		auto evnt = std::static_pointer_cast<WindowResizeEvent>(e);

		int w, h;
		evnt->getSize(w, h);

		// Recreates the framebuffer
		destroy();

		if (m_type == FB_COLOR_BUFFER)
			initialize(m_type, w, h);
	}
}