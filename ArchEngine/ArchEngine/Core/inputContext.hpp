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
 * Last Modified: 19/04/2018                                                 *
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

		bool mapButtonToAction(SDL_Keycode button, InputAction &action);
		bool mapButtonToState(SDL_Keycode button, InputState &state);
		bool mapAxisToRange(SDL_EventType, InputRange &range);

	private:
		std::map<SDL_Keycode, InputAction> m_actions;
		std::map<SDL_Keycode, InputState> m_states;
		std::map<SDL_EventType, InputRange> m_ranges;

		std::map<InputRange, double> m_sensivities;
	};
}


#endif	// CORE_INPUT_CONTEXT_HPP