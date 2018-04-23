/*===========================================================================*
 * Arch Engine - "Core/inputs.hpp"                                           *
 *                                                                           *
 * The inputs accepted by ArchEngine are divided into actions, states and    *
 * ranges.                                                                   *
 *                                                                           *
 * The actions, states and ranges will be defined as the engine grows. For   *
 * now, there are just some basic actions for debugging.                     *
 *                                                                           *
 * Based in:                                                                 *
 * - (https://www.gamedev.net/articles/programming/                          *
 *    general-and-gameplay-programming/                                      *
 *    designing-a-robust-input-handling-system-for-games-r2975)              *
 * - Game Coding Complete - Mike McShaffry and David Graham                  *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 19/04/2018                                                       *
 * Last Modified: 19/04/2018                                                 *
 *===========================================================================*/


#ifndef CORE_INPUTS_HPP
#define CORE_INPUTS_HPP


namespace Core {
    // Actions happen once, when the button is first pressed or released
    // (For instance, holding down a button only triggers one action).
	enum InputAction {
		INPUT_ACTION_NONE,

		// Window related actions
		INPUT_ACTION_WINDOW_MINIMIZE,
		INPUT_ACTION_WINDOW_MAXIMIZE,

		// System related actions
		INPUT_ACTION_QUIT,

		N_INPUT_ACTIONS
	};

    // States defines when some game event should be on or off.
	enum InputState {
		INPUT_STATE_NONE,

		// Camera related states
		INPUT_STATE_CAMERA_MOVEMENT_FORWARD,
		INPUT_STATE_CAMERA_MOVEMENT_BACKWARD,
		INPUT_STATE_CAMERA_MOVEMENT_LEFT,
		INPUT_STATE_CAMERA_MOVEMENT_RIGHT,
		INPUT_STATE_CAMERA_MOVEMENT_UP,
		INPUT_STATE_CAMERA_MOVEMENT_DOWN,
		INPUT_STATE_CAMERA_MOVEMENT_FAST,

		N_INPUT_STATES
	};

    // Ranges are inputs associated with a value, usefull for mouse
    // representation.
	enum InputRange {
		INPUT_RANGE_NONE,

		// Camera related ranges
		INPUT_RANGE_CAMERA_AXIS_X,
		INPUT_RANGE_CAMERA_AXIS_Y,

		N_INPUT_RANGES
	};

	// Mouse axes should be mapped to input ranges
	enum ControllerAxis {
		MOUSE_AXIS_X,
		MOUSE_AXIS_Y,

		N_AXES
	};
}


#endif	// CORE_INPUS_HPP