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
using namespace Utils;


namespace Core {
	InputContext::InputContext(const std::string& path) {/*
		LuaScript context;
		context.initialize(path);

		std::vector<std::string> keys = context.getTableKeys("context");

		for (auto &it : keys)
			ss << it << ", ";
		ss << "\n";

		ServiceLocator::getFileLogger()->log<LOG_INFO>(ss);

		context.destroy();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLog()->log<LOG_DEBUG>(
			path + " input context constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG*/
	}

	InputContext::~InputContext() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Input context destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	bool InputContext::mapButtonToAction(SDL_Keycode button, InputAction &action) {
		return true;
	}
	bool InputContext::mapButtonToState(SDL_Keycode button, InputState &state) {
		return true;
	}
	bool InputContext::mapAxisToRange(SDL_EventType, InputRange &range) {
		return true;
	}
}