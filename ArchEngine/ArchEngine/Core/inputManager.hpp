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
#include <string>


namespace Core {
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

		// Sends the current input configuration to the engine
		void dispatch();

		void clear(); // Clears all current input

	private:
		InputManager();

		std::map<std::string, InputContext> m_contexts;
		std::vector<InputContext*> m_active_contexts;


	};
}


#endif	// CORE_INPUT_MANAGER_HPP