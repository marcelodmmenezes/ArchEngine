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


#ifndef CORE_INPUT_MANAGER_HPP
#define CORE_INPUT_MANAGER_HPP


#include "inputContext.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>


namespace Core {
	struct CurrentInput {
		// Sets for O(log(n)) access
		std::set<InputAction> m_actions;
		std::set<InputState> m_states;
		std::set<RangeInfo> m_ranges;

		void removeAction(InputAction action);
		void removeState(InputState state);
		void removeRange(RangeInfo range);
	};

	class InputManager {
	public:
		~InputManager();

		InputManager(const InputManager&) = delete;
		void operator=(const InputManager&) = delete;

		static InputManager& getInstance();

		void initialize(const std::string& path);
		void update();
		void destroy();

		// Adds or removes an active input context
		void pushContext(const std::string& context);
		void popContext();

		// Input gathering and clearing
		void setKeyState(SDL_Keycode key, bool pressed, bool prev_pressed);
		void setModState(SDL_Keymod mod, bool pressed, bool prev_pressed);
		void setAxisValue(ControllerAxis axis, double value);
		void clearInput();

		// Engine actions and states triggers
		bool triggerAction(SDL_Keycode key, InputAction& action);
		bool triggerAction(SDL_Keymod mod, InputAction& action);
		bool triggerState(SDL_Keycode key, InputState& state);
		bool triggerState(SDL_Keymod mod, InputState& state);
		void triggerAndConsume(SDL_Keycode key);
		void triggerAndConsume(SDL_Keymod mod);

		// Sends the CurrentInput configuration to the engine
		void dispatch();

	private:
		InputManager();

		std::vector<InputContext> m_contexts;

		// Maps the context name to its position in m_contexts
		std::map<std::string, unsigned> m_mapped_contexts;
		// The indices of the active contexts
		std::vector<unsigned> m_active_contexts;

		CurrentInput m_current_input;
	};
}


#endif	// CORE_INPUT_MANAGER_HPP