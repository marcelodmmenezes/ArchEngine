/*===========================================================================*
 * Arch Engine - "Core/inputContext.hpp"                                     *
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
 * Last Modified: 23/04/2018                                                 *
 *===========================================================================*/


#ifndef CORE_INPUT_CONTEXT_HPP
#define CORE_INPUT_CONTEXT_HPP


#include "inputs.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <map>
#include <string>


namespace Core {
	// Mouse buttons in SDL are just a macro
	typedef int MouseButton;

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

		bool mapKeyToAction(SDL_Keycode key, InputAction& action);
		bool mapKeyToState(SDL_Keycode key, InputState& state);

		bool mapModToAction(SDL_Keymod mod, InputAction& action);
		bool mapModToState(SDL_Keymod mod, InputState& state);

		bool mapMBToAction(MouseButton mb, InputAction& action);
		bool mapMBToState(MouseButton mb, InputState& state);

		bool mapAxisToRange(ControllerAxis axis, RangeInfo& range);
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


#endif	// CORE_INPUT_CONTEXT_HPP