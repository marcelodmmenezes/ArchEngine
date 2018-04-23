/*===========================================================================*
 * Arch Engine - "Core/inputManager.hpp"                                     *
 *                                                                           *
 * Responsible for creating and managing all input associated stuff.         *
 * There's already too much layers of abstraction in the input system, due   *
 * to all the key mapping functionalities, so I opted for using a singleton  *
 * approach instead of using the ServiceLocator.                             *
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
		// Reads the input contexts
		LuaScript lua_context;
		lua_context.initialize(path);

		auto input_contexts = lua_context.getTablePairs("contexts");

		for (auto& it : input_contexts)
			m_contexts.insert(std::make_pair(
				it.first, InputContext(it.second)));

		lua_context.destroy();

		// Once the input contexts are read, the maps in Core::InputNames
		// (inputContext.cpp) are cleared. No more need for them.
		InputNames::clearInputMapping();
	}

	void InputManager::update() {
		// TODO
	}

	void InputManager::destroy() {
		// TODO
	}

	void InputManager::pushContext(const std::string& context) {
		auto it = m_contexts.find(context);

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(it != m_contexts.end());
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		m_active_contexts.push_back(&it->second);
	}

	void InputManager::popContext() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(!m_active_contexts.empty());
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		m_active_contexts.pop_back();
	}
}