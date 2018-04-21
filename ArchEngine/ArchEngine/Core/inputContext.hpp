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
 * Last Modified: 20/04/2018                                                 *
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
	class InputContext {
	public:
		InputContext(const std::string& path);
		~InputContext();

		bool mapKeyToAction(SDL_Keycode key, InputAction& action);
		bool mapKeyToState(SDL_Keycode key, InputState& state);

		bool mapModToAction(SDL_Keymod mod, InputAction& action);
		bool mapModToState(SDL_Keymod mod, InputState& state);

		bool mapAxisToRange(ControllerAxis axis, InputRange& range);

	private:
		struct Range {
			InputRange range;
			int min_input;
			int max_input;
			int min_output;
			int max_output;
			double sensitivity;
		};

		// Key <-> action, key <-> state mapping
		std::map<SDL_Keycode, InputAction> m_key_actions;
		std::map<SDL_Keycode, InputState> m_key_states;

		// Modifier <-> action, modifier <-> state mapping
		std::map<SDL_Keymod, InputAction> m_mod_actions;
		std::map<SDL_Keymod, InputState> m_mod_states;

		// ControllerAxis <-> Range mapping
		std::map<ControllerAxis, Range> m_ranges;
	};
}


#endif	// CORE_INPUT_CONTEXT_HPP