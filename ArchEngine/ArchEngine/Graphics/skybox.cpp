/*===========================================================================*
 * Arch Engine - "Graphics/skybox.hpp"                                       *
 *                                                                           *
 * Skybox.                                                                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/06/2018                                                       *
 * Last Modified: 25/06/2018                                                 *
 *===========================================================================*/


#include "skybox.hpp"


using namespace Utils;


namespace Graphics {
	Skybox::Skybox() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Skybox constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	Skybox::~Skybox() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Skybox destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	void Skybox::initialize(unsigned shader_id, unsigned texture_id) {
		m_shader_id = shader_id;
		m_texture_id = texture_id;
		createVao();
		m_state = INITIALIZED;
	}

	void Skybox::destroy() {
		if (glIsVertexArray(m_vao_id))
			glDeleteVertexArrays(1, &m_vao_id);

		if (glIsBuffer(m_vbo_id))
			glDeleteBuffers(1, &m_vbo_id);

		if (glIsBuffer(m_vbo_id))
			glDeleteBuffers(1, &m_ebo_id);

		m_state = SAFE_TO_DESTROY;
	}
	
	unsigned Skybox::getShaderId() const {
		return m_shader_id;
	}

	unsigned Skybox::getTextureId() const {
		return m_texture_id;
	}

	void Skybox::setFogColor(const glm::vec3& color) {
		m_fog_color = color;
	}

	glm::vec3 Skybox::getFogColor() const {
		return m_fog_color;
	}

	void Skybox::draw(GLenum primitive) const {
		glBindVertexArray(m_vao_id);
		glDrawElements(primitive, NUMBER_OF_INDICES, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Skybox::createVao() {
		float vertices[] = {
			// front
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			// back
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
		};

		float indices[] = {
			// front
			0, 1, 2,
			2, 3, 0,
			// top
			3, 2, 6,
			6, 7, 3,
			// back
			7, 6, 5,
			5, 4, 7,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// left
			4, 0, 3,
			3, 7, 4,
			// right
			1, 5, 6,
			6, 2, 1,
		};

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
			vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
			indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}