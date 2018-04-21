--[[
/*===========================================================================*
 * Arch Engine - "Config/testContext.lua"                                    *
 *                                                                           *
 * Input context for debugging                                               *
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
]]


-- Using SDL keycodes for mapping. Change to more generic stuff later.

-- The 'context', 'actions', 'states' and 'ranges' name is hardcoded, so it
-- should always be those names. Make this dynamic later.

context = {
	actions = {
		SDLK_ESCAPE = "ACTION_QUIT"
	},

	states = {
		SDLK_w = "STATE_CAMERA_MOVE_FORWARD",
		SDLK_s = "STATE_CAMERA_MOVE_BACKWARD",
		SDLK_a = "STATE_CAMERA_MOVE_LEFT",
		SDLK_d = "STATE_CAMERA_MOVE_RIGHT",
		SDLK_q = "STATE_CAMERA_MOVEMENT_UP",
		SDLK_e = "STATE_CAMERA_MOVEMENT_DOWN",
		SDLK_LSHIFT = "STATE_CAMERA_MOVEMENT_FAST"
	},

	-- Always mapped to mouse motion. Maybe a joystick in the future
	ranges = {
		MOUSE_AXIS_X = {
			range = "RANGE_CAMERA_AXIS_X",
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 50
		},
		MOUSE_AXIS_Y = {
			range = "RANGE_CAMERA_AXIS_Y",
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 50
		}
	}
}