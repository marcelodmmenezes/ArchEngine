/*===========================================================================*
 * Arch Engine - "OS/inputContext.hpp"                                       *
 *                                                                           *
 * Represents the input context                                              *
 *                                                                           *
 * Based in:                                                                 *
 * - (https://www.gamedev.net/articles/programming/                          *
 *    general-and-gameplay-programming/                                      *
 *    designing-a-robust-input-handling-system-for-games-r2975)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 19/04/2018                                                       *
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#ifndef OS_INPUT_CONTEXT_HPP
#define OS_INPUT_CONTEXT_HPP


#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <map>
#include <string>


/*
 * The inputs accepted by ArchEngine are divided into actions, states and
 * ranges, which I'll call input events.
 *
 * These input events are represented simple as integers. The keys associated
 * with each event are described at the lua input context scripts, and it's up
 * to the game programmer to define the key/event relationship. Later, he can
 * define a game event associated with each integer, through enumarations for
 * instance, making the input system very customizable.

 * - Actions happen once, when the button is first pressed or released
 * (for instance, holding down a button only triggers one action).
 *
 * - States defines when some game event should be on or off
 * (represents continuous events by holding down buttons, for instance).
 *
 * - Ranges are inputs associated with a value, usefull for analogic controls
 * (like mouse and joystick) representation.
 */


namespace OS {
	// Mouse buttons in SDL are just a macro.
	typedef int MouseButton;

	// As SDL doesn't define axes, they're defined here.
	enum ControllerAxis {
		MOUSE_AXIS_X,
		MOUSE_AXIS_Y,
		MOUSE_WHEEL,

		N_AXES
	};

	// Typedefs for readability
	typedef int InputAction;
	typedef int InputState;
	typedef int InputRange;

	namespace InputNames {
		void clearInputMapping();
	}

	struct RangeInfo {
		RangeInfo();

		bool operator<(const RangeInfo& ri) const;

		void calc(double value);

		InputRange m_range;
		double m_min_input;
		double m_max_input;
		double m_min_output;
		double m_max_output;
		double m_sensitivity;

		// Stores the current value of the range
		double m_value;
	};

	class InputContext {
	public:
		InputContext(const std::string& path);
		~InputContext();

		// These mapping methods return 'true' if the current context
		// has an input event associated with the given key,
		// key modifier, mouse button or controller axis.
		// Returns 'false' otherwise.

		bool mapKeyToAction(SDL_Keycode key, InputAction& action);
		bool mapKeyToState(SDL_Keycode key, InputState& state);

		bool mapModToAction(SDL_Keymod mod, InputAction& action);
		bool mapModToState(SDL_Keymod mod, InputState& state);

		bool mapMBToAction(MouseButton mb, InputAction& action);
		bool mapMBToState(MouseButton mb, InputState& state);

		bool mapAxisToRange(ControllerAxis axis, RangeInfo& range);

		// Returns the given axis sensitivity (Captain Obvious!)
		double getAxisSensitivity(ControllerAxis axis);

	private:
		// Key <-> action, key <-> state mapping
		std::map<SDL_Keycode, InputAction> m_key_actions;
		std::map<SDL_Keycode, InputState> m_key_states;

		// Modifier <-> action, modifier <-> state mapping
		std::map<SDL_Keymod, InputAction> m_mod_actions;
		std::map<SDL_Keymod, InputState> m_mod_states;

		// Mouse buttons <-> action, Mouse buttons <-> state mapping
		std::map<MouseButton, InputAction> m_mb_actions;
		std::map<MouseButton, InputState> m_mb_states;

		// ControllerAxis <-> Range mapping
		std::map<ControllerAxis, RangeInfo> m_ranges;
	};
}


#endif	// OS_INPUT_CONTEXT_HPP