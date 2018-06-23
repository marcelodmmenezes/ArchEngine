--[[
/*===========================================================================*
 * Arch Engine - "Testers/testContext.lua"                                   *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 21/06/2018                                                 *
 *===========================================================================*/
]]


context = {
	-- Mapping characters to its ASCII values
	actions = {
		SDLK_LSHIFT = 0,
		SDLK_BACKSPACE = 8,
		SDLK_TAB = 9,
		SDLK_RETURN = 13,
		SDLK_ESCAPE = 27,

		SDLK_SPACE = 32,
		SDLK_EXCLAIM = 33,
		SDLK_QUOTEDBL = 34,
		SDLK_HASH = 35,
		SDLK_DOLLAR = 36,
		SDLK_PERCENT = 37,
		SDLK_AMPERSAND = 38,
		SDLK_QUOTE = 39,
		SDLK_LEFTPAREN = 40,
		SDLK_RIGHTPAREN = 41,
		SDLK_ASTERISK = 42,
		SDLK_PLUS = 43,
		SDLK_COMMA = 44,
		SDLK_MINUS = 45,
		SDLK_PERIOD = 46,
		SDLK_SLASH = 47,
		SDLK_0 = 48,
		SDLK_1 = 49,
		SDLK_2 = 50,
		SDLK_3 = 51,
		SDLK_4 = 52,
		SDLK_5 = 53,
		SDLK_6 = 54,
		SDLK_7 = 55,
		SDLK_8 = 56,
		SDLK_9 = 57,
		SDLK_COLON = 58,
		SDLK_SEMICOLON = 59,
		SDLK_LESS = 60,
		SDLK_EQUALS = 61,
		SDLK_GREATER = 62,
		SDLK_QUESTION = 63,
		SDLK_AT = 64,

		SDLK_LEFTBRACKET = 91,
		SDLK_BACKSLASH = 92,
		SDLK_RIGHTBRACKET = 93,

		SDLK_UNDERSCORE = 95,

		--SDLK_KP_LEFTBRACE = 173,
		--SDLK_KP_VERTICALBAR = 174,
		--SDLK_KP_RIGHTBRACE = 175,

		SDLK_a = 97,
		SDLK_b = 98,
		SDLK_c = 99,
		SDLK_d = 100,
		SDLK_e = 101,
		SDLK_f = 102,
		SDLK_g = 103,
		SDLK_h = 104,
		SDLK_i = 105,
		SDLK_j = 106,
		SDLK_k = 107,
		SDLK_l = 108,
		SDLK_m = 109,
		SDLK_n = 110,
		SDLK_o = 111,
		SDLK_p = 112,
		SDLK_q = 113,
		SDLK_r = 114,
		SDLK_s = 115,
		SDLK_t = 116,
		SDLK_u = 117,
		SDLK_v = 118,
		SDLK_w = 119,
		SDLK_x = 120,
		SDLK_y = 121,
		SDLK_z = 122,
	},

	states = {
		SDL_BUTTON_LEFT = 7,
		SDL_BUTTON_RIGHT = 8,
	},
}