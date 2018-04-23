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
	//------------------------------------------------------------ CurrentInput
	void CurrentInput::removeAction(InputAction action) {
		m_actions.erase(action);
	}

	void CurrentInput::removeState(InputState state) {
		m_states.erase(state);
	}

	void CurrentInput::removeRange(RangeInfo range) {
		m_ranges.erase(range);
	}

	//------------------------------------------------------------ InputManager
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

		for (auto& it : input_contexts) {
			m_contexts.push_back(InputContext(it.second));
			m_mapped_contexts.insert(std::make_pair(
				it.first, m_contexts.size() - 1));
		}

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
		auto it = m_mapped_contexts.find(context);

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(it != m_mapped_contexts.end());
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		m_active_contexts.push_back(it->second);
	}

	void InputManager::popContext() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(!m_active_contexts.empty());
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		m_active_contexts.pop_back();
	}

	void InputManager::setKeyState(SDL_Keycode key,
		bool pressed, bool prev_pressed) {
		InputAction action;
		InputState state;

		if (pressed && !prev_pressed) {
			if (triggerAction(key, action)) {
				m_current_input.m_actions.insert(action);
				return;
			}
		}

		if (pressed) {
			if (triggerState(key, state)) {
				m_current_input.m_states.insert(state);
				return;
			}
		}

		triggerAndConsume(key);
	}

	void InputManager::setModState(SDL_Keymod mod,
		bool pressed, bool prev_pressed) {
		InputAction action;
		InputState state;

		if (pressed && !prev_pressed) {
			if (triggerAction(mod, action)) {
				m_current_input.m_actions.insert(action);
				return;
			}
		}

		if (pressed) {
			if (triggerState(mod, state)) {
				m_current_input.m_states.insert(state);
				return;
			}
		}

		triggerAndConsume(mod);
	}

	void InputManager::setAxisValue(ControllerAxis axis, double value) {
		for (unsigned it : m_active_contexts) {
			RangeInfo range;

			if (m_contexts[it].mapAxisToRange(axis, range)) {
				range.calc(value);
				m_current_input.m_ranges.insert(range);
				break;
			}
		}
	}

	void InputManager::clearInput() {
		m_current_input.m_actions.clear();
		m_current_input.m_ranges.clear();
	}

	bool InputManager::triggerAction(SDL_Keycode key, InputAction& action) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapKeyToAction(key, action))
				return true;

		return false;
	}

	bool InputManager::triggerAction(SDL_Keymod mod, InputAction& action) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapKeyToAction(mod, action))
				return true;

		return false;
	}

	bool InputManager::triggerState(SDL_Keycode key, InputState& state) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapKeyToState(key, state))
				return true;

		return false;
	}

	bool InputManager::triggerState(SDL_Keymod mod, InputState& state) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapKeyToState(mod, state))
				return true;

		return false;
	}

	void InputManager::triggerAndConsume(SDL_Keycode key) {
		InputAction action;
		InputState state;

		if (triggerAction(key, action))
			m_current_input.removeAction(action);

		if (triggerState(key, state))
			m_current_input.removeState(state);
	}

	void InputManager::triggerAndConsume(SDL_Keymod mod) {
		InputAction action;
		InputState state;

		if (triggerAction(mod, action))
			m_current_input.removeAction(action);

		if (triggerState(mod, state))
			m_current_input.removeState(state);
	}

	void InputManager::dispatch() {
		// TODO
	}
}