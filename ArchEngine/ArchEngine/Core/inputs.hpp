/*===========================================================================*
 * Arch Engine - "Core/inputs.hpp"                                           *
 *                                                                           *
 * The inputs accepted by ArchEngine are divided into actions, states and    *
 * ranges.                                                                   *
 * - Actions happen once, when the button is first pressed or released       *
 * (For instance, holding down a button only triggers one action).           *
 * - States defines when some game event should be on or off.                *
 * - Ranges are inputs associated with a value, usefull for mouse            *
 * representation.                                                           *
 *                                                                           *
 * The actions, states and ranges will be defined as the engine grows. For   *
 * now, there are just some basic actions for debugging.                     *
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


#ifndef CORE_INPUTS_HPP
#define CORE_INPUTS_HPP


namespace Core {
	enum Action {
		// Camera related actions
		ACTION_CAMERA_MOVEMENT_FORWARD,
		ACTION_CAMERA_MOVEMENT_BACKWARD,
		ACTION_CAMERA_MOVEMENT_LEFT,
		ACTION_CAMERA_MOVEMENT_RIGHT,
		ACTION_CAMERA_MOVEMENT_UP,
		ACTION_CAMERA_MOVEMENT_DOWN,

		// System related actions
		ACTION_QUIT
	}

	enum State {
		// Camera related states
		STATE_CAMERA_FAST
	}

	enum Range {
		// Camera related ranges
		RANGE_CAMERA_AXIS_X,
		RANGE_CAMERA_AXIS_Y
	}
}


#endif	// CORE_INPUS_HPP