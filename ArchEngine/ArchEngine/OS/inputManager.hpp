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
 * Last Modified: 11/06/2018                                                 *
 *===========================================================================*/


#ifndef OS_INPUT_MANAGER_HPP
#define OS_INPUT_MANAGER_HPP


#include "inputContext.hpp"
#include "../Core/eventManager.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/fileWatcher.hpp"
#include "../Utils/serviceLocator.hpp"

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <vector>


//------------------------
//--- Lua InputManager API
//------------------------
extern "C" {
	int pushContext(lua_State* lua);
	int popContext(lua_State* lua);
}
//-------------------------------------------------------------------------


namespace OS {
	//----------------
	//--- Input events
	//----------------

	class InputActionEvent : public Core::IEvent {
	public:
		InputActionEvent();
		InputActionEvent(OS::InputAction value);
		~InputActionEvent();

		Core::EventType getType() const override;
		InputAction getValue() const;

	private:
		InputAction m_value;
	};

	class InputStateEvent : public Core::IEvent {
	public:
		InputStateEvent();
		InputStateEvent(InputState value, bool over = false);
		~InputStateEvent();

		Core::EventType getType() const override;
		InputState getValue() const;
		bool isOver() const;

	private:
		InputState m_value;
		bool m_over;
	};

	class InputRangeEvent : public Core::IEvent {
	public:
		InputRangeEvent();
		InputRangeEvent(const RangeInfo& value);
		~InputRangeEvent();

		Core::EventType getType() const override;
		RangeInfo getValue() const;

	private:
		RangeInfo m_value;
	};

	class InputMouseMoved : public Core::IEvent {
	public:
		InputMouseMoved(int x, int y);
		~InputMouseMoved();

		Core::EventType getType() const override;
		void getValues(int& x, int&y) const;

	private:
		int m_x;
		int m_y;
	};

	class InputContextEvent : public Core::IEvent {
	public:
		InputContextEvent();
		InputContextEvent(const std::string& name, bool state);
		~InputContextEvent();

		Core::EventType getType() const override;

		std::string getName() const;
		void setName(const std::string& name);

		bool getState() const;
		void setState(bool state);

	private:
		// true for pushed, false for popped
		bool m_state;
		std::string m_name;
	};
	//-------------------------------------------------------------------------

	struct CurrentInput {
		// Sets for O(log(n)) access
		std::set<InputAction> m_actions;
		std::set<InputState> m_states;
		std::vector<InputState> m_over_states;
		std::set<RangeInfo> m_ranges;
	};

	class InputManager {
	public:
		~InputManager();

		InputManager(const InputManager&) = delete;
		void operator=(const InputManager&) = delete;

		static InputManager& getInstance();

		bool initialize(const std::string& path);

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		void onFileModifiedEvent(Core::EventPtr e);
		void watchInputManager(bool watch);
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

		// Gathers and maps the events from the operating system
		void update(bool& running);

		// Adds or removes an active input context
		void pushContext(const std::string& context);
		void popContext(const std::string& context);

	private:
		InputManager();

		// Input gathering
		void setKeyState(SDL_Keycode key, bool pressed, bool prev_pressed);
		void setModState(SDL_Keymod mod, bool pressed, bool prev_pressed);
		void setMBState(MouseButton mb, bool pressed, bool prev_pressed);
		void setAxisValue(ControllerAxis axis, double value);

		// Sends the CurrentInput configuration to the engine
		void dispatch();

		// Actions and states triggers
		bool triggerKeyAction(SDL_Keycode key, InputAction& action);
		bool triggerModAction(SDL_Keymod mod, InputAction& action);
		bool triggerMBAction(MouseButton mb, InputAction& action);

		bool triggerKeyState(SDL_Keycode key, InputState& state);
		bool triggerModState(SDL_Keymod mod, InputState& state);
		bool triggerMBState(MouseButton mb, InputState& state);

		// Force actions, states or ranges to happen
		void forceAction(InputAction action);
		void forceState(InputState state);
		void forceRangeInfo(RangeInfo range);

		// Mouse control variables
		// Auxiliary to make right calc when mouse first enters the screen
		bool m_mouse_first; 
		int m_mouse_last_x;
		int m_mouse_last_y;

		// List of sdl key modifiers for iteration,
		// for it's not a sequential enumeration.
		SDL_Keymod m_sdl_modifiers[12] = {
			KMOD_LSHIFT, KMOD_RSHIFT, KMOD_LCTRL, KMOD_RCTRL,
			KMOD_LALT, KMOD_RALT, KMOD_LGUI, KMOD_RGUI,
			KMOD_NUM, KMOD_CAPS, KMOD_MODE, KMOD_RESERVED
		};

		// Stores the previous key states (true = pressed, false = released)
		std::map<SDL_Keycode, bool> m_key_prev_state;
		// Stores the previous modifier states
		std::map<SDL_Keymod, bool> m_mod_prev_state;
		// Stores the previous mouse button states
		std::map<MouseButton, bool> m_mb_prev_state;

		// Input contexts
		std::vector<InputContext> m_contexts;
		// Maps the context name to its index in m_contexts
		std::map<std::string, unsigned> m_mapped_contexts;
		// The indices of the active contexts
		std::vector<unsigned> m_active_contexts;
		// Stores the actions, states and ranges that happen at each frame
		CurrentInput m_current_input;

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		// Hot-reloading configuration
		std::string m_config_file_path;
		bool m_watch_file, m_file_being_watched;
		Core::EventListener m_file_modified_listener;
#endif	// ARCH_ENGINE_HOT_RELOAD_ON
	};
}


#endif	// OS_INPUT_MANAGER_HPP