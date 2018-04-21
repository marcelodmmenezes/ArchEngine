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


#include "inputContext.hpp"


using namespace Script;
using namespace Utils;


namespace Core {
	namespace InputNames {
		// Remember to clear maps after input reading to save memory
		// String <-> SDL_Keycode mapping
		std::map<std::string, SDL_Keycode> keycode_names = {
			// SDL_Keycode
			std::make_pair("SDLK_UNKNOWN", SDLK_UNKNOWN),
			std::make_pair("SDLK_RETURN", SDLK_RETURN),
			std::make_pair("SDLK_ESCAPE", SDLK_ESCAPE),
			std::make_pair("SDLK_BACKSPACE", SDLK_BACKSPACE),
			std::make_pair("SDLK_TAB", SDLK_TAB),
			std::make_pair("SDLK_SPACE", SDLK_SPACE),
			std::make_pair("SDLK_EXCLAIM", SDLK_EXCLAIM),
			std::make_pair("SDLK_QUOTEDBL", SDLK_QUOTEDBL),
			std::make_pair("SDLK_HASH", SDLK_HASH),
			std::make_pair("SDLK_PERCENT", SDLK_PERCENT),
			std::make_pair("SDLK_DOLLAR", SDLK_DOLLAR),
			std::make_pair("SDLK_AMPERSAND", SDLK_AMPERSAND),
			std::make_pair("SDLK_QUOTE", SDLK_QUOTE),
			std::make_pair("SDLK_LEFTPAREN", SDLK_LEFTPAREN),
			std::make_pair("SDLK_RIGHTPAREN", SDLK_RIGHTPAREN),
			std::make_pair("SDLK_ASTERISK", SDLK_ASTERISK),
			std::make_pair("SDLK_PLUS", SDLK_PLUS),
			std::make_pair("SDLK_COMMA", SDLK_COMMA),
			std::make_pair("SDLK_MINUS", SDLK_MINUS),
			std::make_pair("SDLK_PERIOD", SDLK_PERIOD),
			std::make_pair("SDLK_SLASH", SDLK_SLASH),
			std::make_pair("SDLK_0", SDLK_0),
			std::make_pair("SDLK_1", SDLK_1),
			std::make_pair("SDLK_2", SDLK_2),
			std::make_pair("SDLK_3", SDLK_3),
			std::make_pair("SDLK_4", SDLK_4),
			std::make_pair("SDLK_5", SDLK_5),
			std::make_pair("SDLK_6", SDLK_6),
			std::make_pair("SDLK_7", SDLK_7),
			std::make_pair("SDLK_8", SDLK_8),
			std::make_pair("SDLK_9", SDLK_9),
			std::make_pair("SDLK_COLON", SDLK_COLON),
			std::make_pair("SDLK_SEMICOLON", SDLK_SEMICOLON),
			std::make_pair("SDLK_LESS", SDLK_LESS),
			std::make_pair("SDLK_EQUALS", SDLK_EQUALS),
			std::make_pair("SDLK_GREATER", SDLK_GREATER),
			std::make_pair("SDLK_QUESTION", SDLK_QUESTION),
			std::make_pair("SDLK_AT", SDLK_AT),

			std::make_pair("SDLK_LEFTBRACKET", SDLK_LEFTBRACKET),
			std::make_pair("SDLK_BACKSLASH", SDLK_BACKSLASH),
			std::make_pair("SDLK_RIGHTBRACKET", SDLK_RIGHTBRACKET),
			std::make_pair("SDLK_CARET", SDLK_CARET),
			std::make_pair("SDLK_UNDERSCORE", SDLK_UNDERSCORE),
			std::make_pair("SDLK_BACKQUOTE", SDLK_BACKQUOTE),
			std::make_pair("SDLK_a", SDLK_a),
			std::make_pair("SDLK_b", SDLK_b),
			std::make_pair("SDLK_c", SDLK_c),
			std::make_pair("SDLK_d", SDLK_d),
			std::make_pair("SDLK_e", SDLK_e),
			std::make_pair("SDLK_f", SDLK_f),
			std::make_pair("SDLK_g", SDLK_g),
			std::make_pair("SDLK_h", SDLK_h),
			std::make_pair("SDLK_i", SDLK_i),
			std::make_pair("SDLK_j", SDLK_j),
			std::make_pair("SDLK_k", SDLK_k),
			std::make_pair("SDLK_l", SDLK_l),
			std::make_pair("SDLK_m", SDLK_m),
			std::make_pair("SDLK_n", SDLK_n),
			std::make_pair("SDLK_o", SDLK_o),
			std::make_pair("SDLK_p", SDLK_p),
			std::make_pair("SDLK_q", SDLK_q),
			std::make_pair("SDLK_r", SDLK_r),
			std::make_pair("SDLK_s", SDLK_s),
			std::make_pair("SDLK_t", SDLK_t),
			std::make_pair("SDLK_u", SDLK_u),
			std::make_pair("SDLK_v", SDLK_v),
			std::make_pair("SDLK_w", SDLK_w),
			std::make_pair("SDLK_x", SDLK_x),
			std::make_pair("SDLK_y", SDLK_y),
			std::make_pair("SDLK_z", SDLK_z),

			// SDL_SCANCODED Keys
			std::make_pair("SDLK_CAPSLOCK", SDLK_CAPSLOCK),

			std::make_pair("SDLK_F1", SDLK_F1),
			std::make_pair("SDLK_F2", SDLK_F2),
			std::make_pair("SDLK_F3", SDLK_F3),
			std::make_pair("SDLK_F4", SDLK_F4),
			std::make_pair("SDLK_F5", SDLK_F5),
			std::make_pair("SDLK_F6", SDLK_F6),
			std::make_pair("SDLK_F7", SDLK_F7),
			std::make_pair("SDLK_F8", SDLK_F8),
			std::make_pair("SDLK_F9", SDLK_F9),
			std::make_pair("SDLK_F10", SDLK_F10),
			std::make_pair("SDLK_F11", SDLK_F11),
			std::make_pair("SDLK_F12", SDLK_F12),

			std::make_pair("SDLK_PRINTSCREEN", SDLK_PRINTSCREEN),
			std::make_pair("SDLK_SCROLLLOCK", SDLK_SCROLLLOCK),
			std::make_pair("SDLK_PAUSE", SDLK_PAUSE),
			std::make_pair("SDLK_INSERT", SDLK_INSERT),
			std::make_pair("SDLK_HOME", SDLK_HOME),
			std::make_pair("SDLK_PAGEUP", SDLK_PAGEUP),
			std::make_pair("SDLK_DELETE", SDLK_DELETE),
			std::make_pair("SDLK_END", SDLK_END),
			std::make_pair("SDLK_PAGEDOWN", SDLK_PAGEDOWN),
			std::make_pair("SDLK_RIGHT", SDLK_RIGHT),
			std::make_pair("SDLK_LEFT", SDLK_LEFT),
			std::make_pair("SDLK_DOWN", SDLK_DOWN),
			std::make_pair("SDLK_UP", SDLK_UP),

			std::make_pair("SDLK_LCTRL", SDLK_LCTRL),
			std::make_pair("SDLK_LSHIFT", SDLK_LSHIFT),
			std::make_pair("SDLK_LALT", SDLK_LALT),
			std::make_pair("SDLK_LGUI", SDLK_LGUI),
			std::make_pair("SDLK_RCTRL", SDLK_RCTRL),
			std::make_pair("SDLK_RSHIFT", SDLK_RSHIFT),
			std::make_pair("SDLK_RALT", SDLK_RALT),
			std::make_pair("SDLK_RGUI", SDLK_RGUI)
		};

		// String <-> SDL_Keymod mapping
		std::map<std::string, SDL_Keymod> keymod_names = {
			std::make_pair("KMOD_NONE", KMOD_NONE),
			std::make_pair("KMOD_LSHIFT", KMOD_LSHIFT),
			std::make_pair("KMOD_RSHIFT", KMOD_RSHIFT),
			std::make_pair("KMOD_LCTRL", KMOD_LCTRL),
			std::make_pair("KMOD_RCTRL", KMOD_RCTRL),
			std::make_pair("KMOD_LALT", KMOD_LALT),
			std::make_pair("KMOD_RALT", KMOD_RALT),
			std::make_pair("KMOD_LGUI", KMOD_LGUI),
			std::make_pair("KMOD_RGUI", KMOD_RGUI),
			std::make_pair("KMOD_NUM", KMOD_NUM),
			std::make_pair("KMOD_CAPS", KMOD_CAPS),
			std::make_pair("KMOD_MODE", KMOD_MODE),
			std::make_pair("KMOD_RESERVED", KMOD_RESERVED)
		};

		// String <-> contoller_axis mapping
		std::map<std::string, ControllerAxis> controller_axis = {
			std::make_pair("MOUSE_AXIS_X", MOUSE_AXIS_X),
			std::make_pair("MOUSE_AXIS_Y", MOUSE_AXIS_Y)
		};

		// String <-> engine_actions mapping
		std::map<std::string, InputAction> engine_actions = {
			std::make_pair("INPUT_ACTION_NONE", INPUT_ACTION_NONE),
			std::make_pair("INPUT_ACTION_WINDOW_MINIMIZE",
				INPUT_ACTION_WINDOW_MINIMIZE),
			std::make_pair("INPUT_ACTION_WINDOW_MAXIMIZE",
				INPUT_ACTION_WINDOW_MAXIMIZE),
			std::make_pair("INPUT_ACTION_QUIT", INPUT_ACTION_QUIT),
		};

		// String <-> engine_states mapping
		std::map<std::string, InputState> engine_states = {
			std::make_pair("INPUT_STATE_NONE", INPUT_STATE_NONE),
			std::make_pair("INPUT_STATE_CAMERA_MOVEMENT_FORWARD",
				INPUT_STATE_CAMERA_MOVEMENT_FORWARD),
			std::make_pair("INPUT_STATE_CAMERA_MOVEMENT_BACKWARD",
				INPUT_STATE_CAMERA_MOVEMENT_BACKWARD),
			std::make_pair("INPUT_STATE_CAMERA_MOVEMENT_LEFT",
				INPUT_STATE_CAMERA_MOVEMENT_LEFT),
			std::make_pair("INPUT_STATE_CAMERA_MOVEMENT_RIGHT",
				INPUT_STATE_CAMERA_MOVEMENT_RIGHT),
			std::make_pair("INPUT_STATE_CAMERA_MOVEMENT_UP",
				INPUT_STATE_CAMERA_MOVEMENT_UP),
			std::make_pair("INPUT_STATE_CAMERA_MOVEMENT_DOWN",
				INPUT_STATE_CAMERA_MOVEMENT_DOWN),
			std::make_pair("INPUT_STATE_CAMERA_MOVEMENT_FAST",
				INPUT_STATE_CAMERA_MOVEMENT_FAST),
		};

		// String <-> engine_ranges mapping
		std::map<std::string, InputRange> engine_ranges = {
			std::make_pair("INPUT_RANGE_NONE", INPUT_RANGE_NONE),
			std::make_pair("INPUT_RANGE_CAMERA_AXIS_X",
				INPUT_RANGE_CAMERA_AXIS_X),
			std::make_pair("INPUT_RANGE_CAMERA_AXIS_Y",
				INPUT_RANGE_CAMERA_AXIS_Y),
		};
	}

	//------------------------------------------------------------ InputContext
	InputContext::InputContext(const std::string& path) {
		LuaScript context;
		context.initialize(path);

		// Gets the mapping of SDL to engine states
		std::vector<std::pair<std::string, std::string>> mapping =
			context.getTablePairs("context.states");

		for (auto &input : mapping) {
			// Tries to find action
			auto key_it = InputNames::keycode_names.find(input.first);

			if (key_it != InputNames::keycode_names.end()) {

				continue; // If found, continues the iteration
			}

			// Tries to find state
			auto mod_it = InputNames::keymod_names.find(input.first);

			if (mod_it != InputNames::keymod_names.end()) {
				continue; // If found, continues the iteration
			}

			// Tries to find controller axis
			auto ca_it = InputNames::controller_axis.find(input.first);

			if (ca_it != InputNames::controller_axis.end()) {
				continue; // If found, continues the iteration
			}

			// If none was found the input file is wrong
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
			assert(false);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not map " + path + " input context");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		}



		context.destroy();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			path + " input context constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	InputContext::~InputContext() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Input context destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	bool InputContext::mapKeyToAction(SDL_Keycode key, InputAction& action) {
		auto it = m_key_actions.find(key);

		if (it != m_key_actions.end()) {
			action = it->second;
			return true;
		}

		return false;
	}

	bool InputContext::mapKeyToState(SDL_Keycode key, InputState& state) {
		auto it = m_key_states.find(key);

		if (it != m_key_states.end()) {
			state = it->second;
			return true;
		}

		return false;
	}

	bool InputContext::mapModToAction(SDL_Keymod mod, InputAction& action) {
		auto it = m_mod_actions.find(mod);

		if (it != m_mod_actions.end()) {
			action = it->second;
			return true;
		}

		return false;
	}

	bool InputContext::mapModToState(SDL_Keymod mod, InputState& state) {
		auto it = m_mod_states.find(mod);

		if (it != m_mod_states.end()) {
			state = it->second;
			return true;
		}

		return false;
	}

	bool InputContext::mapAxisToRange(ControllerAxis axis, InputRange& range) {
		// TODO
		/*
		auto it = m_ranges.find(axis);

		if (it != m_ranges.end()) {
			range = it->second;
			return true;
		}

		return false;*/
		return true;
	}
}