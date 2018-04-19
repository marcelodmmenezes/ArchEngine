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


#include "inputContext.hpp"


using namespace Script;


namespace Core {
	InputContext(const std::string& path) {
		Lua

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLog()->log<LOG_DEBUG>(
			path + " input context constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	~InputContext() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLog()->log<LOG_DEBUG>(
			"Input context destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	bool mapButtonToAction(SDL_Keycode button, InputAction &action) {

	}
	bool mapButtonToState(SDL_Keycode button, InputState &state) {

	}
	bool mapAxisToRange(SDL_EventType, InputRange &range) {

	}
}