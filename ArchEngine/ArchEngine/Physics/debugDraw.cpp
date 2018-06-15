/*===========================================================================*
 * Arch Engine - "Physics/debugDraw.cpp"                                     *
 *                                                                           *
 * Class responsible to Bullet debugging draws.                              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 11/06/2018                                                       *
 * Last Modified: 14/06/2018                                                 *
 *===========================================================================*/


#include "debugDraw.hpp"


using namespace Graphics;
using namespace Utils;


namespace Physics {
	DebugDrawer::DebugDrawer(unsigned shader_id) :
		m_created(false), m_shader_id(shader_id), m_debug_mode(0) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"DebugDraw constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	DebugDrawer::~DebugDrawer() {
		if (glIsVertexArray(m_line_vao))
			glDeleteVertexArrays(1, &m_line_vao);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"DebugDraw destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	void DebugDrawer::drawLine(const btVector3& from, const btVector3& to,
		const btVector3& color) {
		m_lines.push_back(
			{
				glm::vec3(from.getX(), from.getY(), from.getZ()),
				glm::vec3(color.getX(), color.getY(), color.getZ())
			}
		);
		m_lines.push_back(
			{
				glm::vec3(to.getX(), to.getY(), to.getZ()),
				glm::vec3(color.getX(), color.getY(), color.getZ())
			}
		);
	}
	
	// TODO
	void DebugDrawer::drawContactPoint(const btVector3& PointOnB,
		const btVector3& normalOnB, btScalar distance, int lifeTime,
		const btVector3& color) {}
	void DebugDrawer::reportErrorWarning(const char* warningString) {}
	void DebugDrawer::draw3dText(const btVector3& location,
		const char* textString) {}

	void DebugDrawer::setDebugMode(int debugMode) {
		m_debug_mode = debugMode;
	}

	int DebugDrawer::getDebugMode() const {
		return m_debug_mode;
	}

	void DebugDrawer::update() {
		if (!m_created) {
			glGenVertexArrays(1, &m_line_vao);
			m_created = true;
		}

		// If no lines were added
		if (m_lines.size() < 2)
			return;

		Shader* shader_ptr =
			GraphicsManager::getInstance().getShader(m_shader_id);

		shader_ptr->bind();
		shader_ptr->setMat4("u_projection_matrix",
			GraphicsManager::getInstance().getProjectionMatrix());
		shader_ptr->setMat4("u_view_matrix",
			GraphicsManager::getInstance().getActiveCamera()->getViewMatrix());
		shader_ptr->update();

		glBindVertexArray(m_line_vao);

		glGenBuffers(1, &m_line_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_line_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_lines.size() *
			sizeof(DebugDrawerVertex), &m_lines[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(DebugDrawerVertex),
			(void*)offsetof(DebugDrawerVertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(DebugDrawerVertex),
			(void*)offsetof(DebugDrawerVertex, color));

		glDrawArrays(GL_LINES, 0, m_lines.size());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteBuffers(1, &m_line_vbo);

		m_lines.clear();
	}
}