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
 * Last Modified: 23/04/2018                                                 *
 *===========================================================================*/
]]


-- Using SDL keycodes for mapping. Change to more generic stuff later.

-- The 'context', 'actions', 'states' and 'ranges' name is hardcoded, so it
-- should always be those names. Make this dynamic later.

context = {
	actions = {
		SDLK_ESCAPE = "INPUT_ACTION_QUIT"
	},

	states = {
		SDLK_w = "INPUT_STATE_CAMERA_MOVEMENT_FORWARD",
		SDLK_s = "INPUT_STATE_CAMERA_MOVEMENT_BACKWARD",
		SDLK_a = "INPUT_STATE_CAMERA_MOVEMENT_LEFT",
		SDLK_d = "INPUT_STATE_CAMERA_MOVEMENT_RIGHT",
		SDLK_q = "INPUT_STATE_CAMERA_MOVEMENT_UP",
		SDLK_e = "INPUT_STATE_CAMERA_MOVEMENT_DOWN",
		SDLK_LSHIFT = "INPUT_STATE_CAMERA_MOVEMENT_FAST"
	},

	-- Always mapped to mouse motion. Maybe a joystick in the future
	ranges = {
		MOUSE_AXIS_X = {
			range = "INPUT_RANGE_CAMERA_AXIS_X",
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 50
		},
		MOUSE_AXIS_Y = {
			range = "INPUT_RANGE_CAMERA_AXIS_Y",
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 50
		},
		MOUSE_WHEEL = {
			range = "INPUT_RANGE_CAMERA_ZOOM",
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 50
		}
	}
}