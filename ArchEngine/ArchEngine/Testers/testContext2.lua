--[[
/*===========================================================================*
 * Arch Engine - "Testers/testContext2.lua"                                  *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 07/05/2018                                                       *
 * Last Modified: 07/05/2018                                                 *
 *===========================================================================*/
]]


context = {
	actions = {
		SDLK_SPACE = 0,
		SDLK_ESCAPE = 2
	},

	states = {
		SDL_BUTTON_LEFT = 0,
		SDL_BUTTON_RIGHT = 1,
		SDL_BUTTON_MIDDLE = 2,
	},

	ranges = {
		MOUSE_AXIS_X = {
			range = 0,
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 50
		},
		MOUSE_AXIS_Y = {
			range = 1,
			min_input = -1000,
			max_input = 1000,
			min_output = -1,
			max_output = 1,
			sensitivity = 50
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