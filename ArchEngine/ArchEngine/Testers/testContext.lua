--[[
/*===========================================================================*
 * Arch Engine - "Testers/testContext.lua"                                   *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 19/04/2018                                                       *
 * Last Modified: 14/06/2018                                                 *
 *===========================================================================*/
]]


-- Using SDL keycodes for mapping. Change to more generic stuff later.

-- The 'context', 'actions', 'states' and 'ranges' name is hardcoded, so it
-- should always be those names. Make this dynamic later.

-- The buttons are mapped to integers by the game programmer, who later defines
-- the game input events based on those integers.

context = {
	actions = {
		SDLK_c = 0,
		SDLK_SPACE = 1,
		SDLK_ESCAPE = 2
	},

	states = {
		SDLK_w = 0,
		SDLK_s = 1,
		SDLK_a = 2,
		SDLK_d = 3,
		SDLK_q = 4,
		SDLK_e = 5,
		SDLK_LSHIFT = 6,
		SDL_BUTTON_LEFT = 7,
		SDL_BUTTON_RIGHT = 8
	},

	ranges = {
		MOUSE_AXIS_X = {
			range = 0,
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 250
		},
		MOUSE_AXIS_Y = {
			range = 1,
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 250
		},
		MOUSE_WHEEL = {
			range = 2,
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 50
		}
	}
}