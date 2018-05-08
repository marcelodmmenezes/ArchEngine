/*===========================================================================*
 * Arch Engine - "OS/inputManager.hpp"                                       *
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
 * Last Modified: 07/05/2018                                                 *
 *===========================================================================*/


#include "inputManager.hpp"


using namespace Core;
using namespace Script;
using namespace Utils;


//---------------------------------------------------- Lua InputManager API
int pushContext(lua_State* lua) {
	int argc = lua_gettop(lua);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"Lua pushing context with " + std::to_string(argc) + " arguments");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
	assert(argc == 1);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

	std::string context(lua_tostring(lua, lua_gettop(lua)));
	lua_pop(lua, 1);

	OS::InputManager::getInstance().pushContext(context);

	// No values returned to Lua
	return 0;
}

int popContext(lua_State* lua) {
	int argc = lua_gettop(lua);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"Lua popping context with " + std::to_string(argc) + " arguments");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
	assert(argc == 1);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

	std::string context(lua_tostring(lua, lua_gettop(lua)));
	lua_pop(lua, 1);

	OS::InputManager::getInstance().popContext(context);

	// No values returned to Lua
	return 0;
}
//-------------------------------------------------------------------------


namespace OS {
	//------------------------------------------------------------ Input events
	InputActionEvent::InputActionEvent() : IEvent(EVENT_INPUT_ACTION) {}
	InputActionEvent::InputActionEvent(InputAction value) :
		IEvent(EVENT_INPUT_ACTION), m_value(value) {}
	InputActionEvent::~InputActionEvent() {}
	EventType InputActionEvent::getType() const { return m_type; }
	InputAction InputActionEvent::getValue() const { return m_value; }
	void InputActionEvent::setValue(InputAction value) { m_value = value; }

	InputStateEvent::InputStateEvent() : IEvent(EVENT_INPUT_STATE) {}
	InputStateEvent::InputStateEvent(InputState value) :
		IEvent(EVENT_INPUT_STATE), m_value(value) {}
	InputStateEvent::~InputStateEvent() {}
	EventType InputStateEvent::getType() const { return m_type; }
	InputState InputStateEvent::getValue() const { return m_value; }
	void InputStateEvent::setValue(InputState value) { m_value = value; }

	InputRangeEvent::InputRangeEvent() : IEvent(EVENT_INPUT_RANGE) {}
	InputRangeEvent::InputRangeEvent(const RangeInfo& value) :
		IEvent(EVENT_INPUT_RANGE), m_value(value) {}
	InputRangeEvent::~InputRangeEvent() {}
	EventType InputRangeEvent::getType() const { return m_type; }
	RangeInfo InputRangeEvent::getValue() const { return m_value; }
	void InputRangeEvent::setValue(const RangeInfo& value) { m_value = value; }

	
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
		m_mouse_last_x(0), m_mouse_last_y(0), m_file_being_watched(false) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Input Manager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	InputManager::~InputManager() {
		EventManager::getInstance().removeListener(
			m_file_modified_listener, EVENT_FILE_MODIFIED);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Input Manager destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	InputManager& InputManager::getInstance() {
		static InputManager instance;
		return instance;
	}

	bool InputManager::initialize(const std::string& path) {
		m_config_file_path = path;

		// Reads the input contexts
		LuaScript lua_context;

		if (!lua_context.initialize(path))
			return false;

		auto input_contexts = lua_context.getTablePairs("contexts");

		for (auto& it : input_contexts) {
			m_contexts.push_back(InputContext(it.second));
			m_mapped_contexts.insert(std::make_pair(
				it.first, m_contexts.size() - 1));
		}

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		if (lua_context.get<bool>("hot_reload")) {
			m_watch_file = true;

			m_file_modified_listener.bind
				<InputManager, &InputManager::onFileModifiedEvent>(this);
			Core::EventManager::getInstance().addListener(
				m_file_modified_listener, Core::EVENT_FILE_MODIFIED);
		}

		// Can't clear the input mapping if we're hot-reloading.
#else
		// Once the input contexts are read, the maps in Core::InputNames
		// (inputContext.cpp) are cleared. No more need for them.
		InputNames::clearInputMapping();
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

		lua_context.destroy();

		return true;
	}

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
	void InputManager::onFileModifiedEvent(Core::EventPtr e) {
		auto evnt = std::static_pointer_cast<FileModifiedEvent>(e);

		// See if the modified file was the InputManager configuration
		if (evnt->getPath() != m_config_file_path)
			return;

		LuaScript lua_context;

		LuaFunctions functions;
		functions.push_back(std::make_pair("pushContext", ::pushContext));
		functions.push_back(std::make_pair("popContext", ::popContext));
		lua_context.initialize(m_config_file_path, functions);

		m_watch_file = lua_context.get<bool>("hot_reload");

		lua_context.destroy();
	}

	void InputManager::watchInputManager(bool watch) {
		m_watch_file = watch;
	}
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

	void InputManager::update(bool& running) {
		SDL_Event sdl_event;

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		// Changes if the file is being watched according to parameters
		if (m_watch_file != m_file_being_watched) {
			Core::EventPtr evnt(new WatchFileEvent(
				m_config_file_path, m_watch_file));
			Core::EventManager::getInstance().postEvent(evnt);
			m_file_being_watched = !m_file_being_watched;
		}
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

		while (SDL_PollEvent(&sdl_event)) {
			switch (sdl_event.type) {
			case SDL_QUIT:
				running = false;
				break;

			case SDL_MOUSEMOTION:
				int x, y;

				if (SDL_GetRelativeMouseMode()) {
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

		dispatch();
	}

	void InputManager::pushContext(const std::string& context) {
		auto it = m_mapped_contexts.find(context);

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(it != m_mapped_contexts.end());
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS
		
		for (auto it2 = m_active_contexts.begin();
			it2 != m_active_contexts.end(); ++it2)
			if (m_contexts[it->second] == m_contexts[*it2])
				return;

		m_active_contexts.push_back(it->second);
	}

	void InputManager::popContext(const std::string& context) {
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

	void InputManager::dispatch() {
		for (auto& it : m_current_input.m_actions) {
			Core::EventPtr evnt(new InputActionEvent(it));
			Core::EventManager::getInstance().postEvent(evnt);
		}

		for (auto& it : m_current_input.m_states) {
			Core::EventPtr evnt(new InputStateEvent(it));
			Core::EventManager::getInstance().postEvent(evnt);
		}

		for (auto& it : m_current_input.m_ranges) {
			Core::EventPtr evnt(new InputRangeEvent(it));
			Core::EventManager::getInstance().postEvent(evnt);
		}

		clearInput();
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