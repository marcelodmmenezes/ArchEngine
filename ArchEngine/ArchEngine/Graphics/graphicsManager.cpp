/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.cpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
 *===========================================================================*/


#include "graphicsManager.hpp"


using namespace Script;
using namespace Utils;


namespace Graphics {
	GraphicsManager::GraphicsManager() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GraphicsManager::~GraphicsManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GraphicsManager& GraphicsManager::getInstance() {
		static GraphicsManager instance;
		return instance;
	}

	bool GraphicsManager::initialize(const glm::vec4& color) {
		// TODO
		glClearColor(color.r, color.g, color.b, color.a);

		return true;
	}

	bool GraphicsManager::initializeFromConfigFile(const std::string& path) {
		// TODO
		return initialize(glm::vec4(0.05f, 0.08f, 0.07f, 1.0f));
	}

	void GraphicsManager::update(float delta_time) {
		// TODO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//---------------------------------------------------------------- TEST
		m_camera.m_delta_time = delta_time;

		m_shader.bind();

		m_shader.setMat4("u_model", glm::mat4(1.0f));
		m_shader.setMat4("u_view", m_camera.getViewMatrix());
		m_shader.setMat4("u_projection", glm::perspective(
			glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
		m_shader.update();

		m_mesh.draw();
		Shader::unbindShaders();
		//---------------------------------------------------------------------
	}

	void GraphicsManager::destroy() {
		// TODO
	}
}