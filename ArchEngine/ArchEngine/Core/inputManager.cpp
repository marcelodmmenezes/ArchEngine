/*===========================================================================*
 * Arch Engine - "Core/inputManager.hpp"                                     *
 *                                                                           *
 * Responsible for creating and handling all input associated stuff.         *
 *                                                                           *
 * Based in:                                                                 *
 * - (https://www.gamedev.net/articles/programming/                          *
 *    general-and-gameplay-programming/                                      *
 *    designing-a-robust-input-handling-system-for-games-r2975)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 23/04/2018                                                       *
 * Last Modified: 23/04/2018                                                 *
 *===========================================================================*/


#include "inputManager.hpp"


using namespace Script;
using namespace Utils;


namespace Core {
	InputManager::InputManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Input Manager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	InputManager::~InputManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Input Manager destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	InputManager& InputManager::getInstance() {
		static InputManager instance;
		return instance;
	}

	void InputManager::initialize(const std::string& path) {
		LuaScript context;
		context.initialize(path);

		auto input_contexts = context.getTablePairs("contexts");

		for (auto& it : input_contexts)
			m_contexts.insert(std::make_pair(
				it.first, InputContext(it.second)));

		context.destroy();
	}

	void InputManager::update() {
		// TODO
	}

	void InputManager::destroy() {
		// TODO
	}
}