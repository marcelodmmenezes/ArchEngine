/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.cpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 13/05/2018                                                 *
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
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsManager::destroy() {
		// TODO
	}
}