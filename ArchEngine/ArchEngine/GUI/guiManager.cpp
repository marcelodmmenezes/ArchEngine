/*===========================================================================*
 * Arch Engine - "Graphics/guiManager.hpp"                                   *
 *                                                                           *
 * Class responsible for managing all the Graphical User Interface           *
 * associated stuff.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/07/2018                                                       *
 * Last Modified: 17/06/2018                                                 *
 *===========================================================================*/


#include "guiManager.hpp"


using namespace Script;
using namespace Utils;


namespace GUI {
	GUIManager::GUIManager() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GUIManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GUIManager::~GUIManager() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GUIManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GUIManager& GUIManager::getInstance() {
		GUIManager instance;
		return instance;
	}

	bool GUIManager::initializeFromConfigFile(const std::string& path) {

	}
}