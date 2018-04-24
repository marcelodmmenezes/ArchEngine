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
 * Last Modified: 24/04/2018                                                 *
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
	InputManager::InputManager() : m_mouse_first(true),
		m_mouse_last_x(0), m_mouse_last_y(0) {
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
		SDL_Event sdl_event;

		while (SDL_PollEvent(&sdl_event)) {
			switch (sdl_event.type) {
			case SDL_QUIT:
				forceAction(0); // Hardcoding for testing
				break;

			case SDL_MOUSEMOTION:
				int x, y;

				if (SDL_GetRelativeMouseMode) {
					SDL_GetRelativeMouseState(&x, &y);

					if (m_mouse_first)
						m_mouse_first = false;
					else {
						setAxisValue(MOUSE_AXIS_X, -x);
						setAxisValue(MOUSE_AXIS_Y, -y);
					}
				}
				else {
					SDL_GetMouseState(&x, &y);
					setAxisValue(MOUSE_AXIS_X, -x);
					setAxisValue(MOUSE_AXIS_Y, -y);
				}
				break;

			case SDL_MOUSEWHEEL:
				setAxisValue(MOUSE_WHEEL, sdl_event.wheel.y);
				break;

			case SDL_MOUSEBUTTONDOWN:
				// Sets the event
				setMBState(sdl_event.button.button, true,
					m_mb_prev_state[sdl_event.button.button]);

				// The prev_state is now 'pressed'
				m_mb_prev_state[sdl_event.button.button] = true;
				break;

			case SDL_MOUSEBUTTONUP:
				// Sets the event
				setMBState(sdl_event.button.button, false,
					m_mb_prev_state[sdl_event.button.button]);

				// The prev_state is now 'released'
				m_mb_prev_state[sdl_event.button.button] = false;
				break;

			case SDL_KEYDOWN:
				// Sets the event
				setKeyState(sdl_event.key.keysym.sym, true,
					m_key_prev_state[sdl_event.key.keysym.sym]);

				// The prev_state is now 'pressed'
				m_key_prev_state[sdl_event.key.keysym.sym] = true;

				for (unsigned i = 0; i < 12; i++) {
					if (sdl_event.key.keysym.mod & m_sdl_modifiers[i]) {
						setModState(m_sdl_modifiers[i], true,
							m_mod_prev_state[m_sdl_modifiers[i]]);
						m_mod_prev_state[m_sdl_modifiers[i]] = true;
					}
				}
				break;

			case SDL_KEYUP:
				// Sets the event
				setKeyState(sdl_event.key.keysym.sym, false,
					m_key_prev_state[sdl_event.key.keysym.sym]);

				// The prev_state is now 'released'
				m_key_prev_state[sdl_event.key.keysym.sym] = false;

				for (unsigned i = 0; i < 12; i++) {
					if (sdl_event.key.keysym.mod & m_sdl_modifiers[i]) {
						setModState(m_sdl_modifiers[i], false,
							m_mod_prev_state[m_sdl_modifiers[i]]);
						m_mod_prev_state[m_sdl_modifiers[i]] = false;
					}
				}
				break;
			}
		}
	}
	
	void InputManager::contextOn(const std::string& context) {
		auto it = m_mapped_contexts.find(context);

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(it != m_mapped_contexts.end());
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		m_active_contexts.push_back(it->second);
	}

	void InputManager::contextOff(const std::string& context) {
		auto it = m_mapped_contexts.find(context);

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(it != m_mapped_contexts.end());
		assert(it->second < m_active_contexts.size());
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		// As the order doesn't matter, we can swap the desired active
		// context with the last and remove the new last, to avoid
		// unnecessary copies.
		std::swap(m_active_contexts[it->second],
			m_active_contexts[m_active_contexts.size() - 1]);

		m_active_contexts.pop_back();
	}

	void InputManager::setKeyState(SDL_Keycode key,
		bool pressed, bool prev_pressed) {
		InputAction action;
		InputState state;

		if (pressed && !prev_pressed && triggerKeyAction(key, action)) {
			m_current_input.m_actions.insert(action);
			return;
		}

		if (pressed && triggerKeyState(key, state)) {
			m_current_input.m_states.insert(state);
			return;
		}

		consumeKey(key);
	}

	void InputManager::setModState(SDL_Keymod mod,
		bool pressed, bool prev_pressed) {
		InputAction action;
		InputState state;

		if (pressed && !prev_pressed && triggerModAction(mod, action)) {
			m_current_input.m_actions.insert(action);
			return;
		}

		if (pressed && triggerModState(mod, state)) {
			m_current_input.m_states.insert(state);
			return;
		}

		consumeMod(mod);
	}

	void InputManager::setMBState(MouseButton mb,
		bool pressed, bool prev_pressed) {
		InputAction action;
		InputState state;

		if (pressed && !prev_pressed && triggerMBAction(mb, action)) {
			m_current_input.m_actions.insert(action);
			return;
		}

		if (pressed && triggerMBState(mb, state)) {
			m_current_input.m_states.insert(state);
			return;
		}

		consumeMB(mb);
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
	
	void InputManager::dispatch(bool& running) {
		// TODO

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		std::stringstream ss;

		ss << "\n\n    INPUTMANAGER DISPATCH:\n    ACTIONS: ";

		for (auto it : m_current_input.m_actions) {
			if (it == 0) // Hardcoded for testing
				running = false;

			ss << it << " ";
		}

		ss << "\n    STATES: ";

		for (auto it : m_current_input.m_states)
			ss << it << " ";

		ss << "\n    RANGES: ";

		for (auto& it : m_current_input.m_ranges)
			ss << it.m_range << " ";

		ss << "\n\n";

		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(ss);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	//--- Action triggers
	bool InputManager::triggerKeyAction(SDL_Keycode key, InputAction& action) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapKeyToAction(key, action))
				return true;

		return false;
	}

	bool InputManager::triggerModAction(SDL_Keymod mod, InputAction& action) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapModToAction(mod, action))
				return true;

		return false;
	}

	bool InputManager::triggerMBAction(MouseButton mb, InputAction& action) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapMBToAction(mb, action))
				return true;

		return false;
	}

	//--- State triggers
	bool InputManager::triggerKeyState(SDL_Keycode key, InputState& state) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapKeyToState(key, state))
				return true;

		return false;
	}

	bool InputManager::triggerModState(SDL_Keymod mod, InputState& state) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapModToState(mod, state))
				return true;

		return false;
	}

	bool InputManager::triggerMBState(MouseButton mb, InputState& state) {
		for (unsigned it : m_active_contexts)
			if (m_contexts[it].mapMBToState(mb, state))
				return true;

		return false;
	}

	//--- Trigger & Consume
	void InputManager::consumeKey(SDL_Keycode key) {
		InputAction action;
		InputState state;

		if (triggerKeyAction(key, action))
			m_current_input.removeAction(action);

		if (triggerKeyState(key, state))
			m_current_input.removeState(state);
	}

	void InputManager::consumeMod(SDL_Keymod mod) {
		InputAction action;
		InputState state;

		if (triggerModAction(mod, action))
			m_current_input.removeAction(action);

		if (triggerModState(mod, state))
			m_current_input.removeState(state);
	}

	void InputManager::consumeMB(MouseButton mb) {
		InputAction action;
		InputState state;

		if (triggerMBAction(mb, action))
			m_current_input.removeAction(action);

		if (triggerMBState(mb, state))
			m_current_input.removeState(state);
	}

	// Force actions, states or ranges to happen
	void InputManager::forceAction(InputAction action) {
		m_current_input.m_actions.insert(action);
	}

	void InputManager::forceState(InputState state) {
		m_current_input.m_states.insert(state);
	}

	void InputManager::forceRangeInfo(RangeInfo range) {
		m_current_input.m_ranges.insert(range);
	}
}