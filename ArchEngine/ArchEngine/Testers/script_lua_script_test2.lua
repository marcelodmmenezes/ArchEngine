--[[
/*===========================================================================*
 * Arch Engine - "Testers/script_lua_script_test2.lua"                       *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 * Based in:                                                                 *
 * - (https://eliasdaler.wordpress.com/2013/10/11/lua_cpp_binder/)           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/04/2018                                                       *
 * Last Modified: 16/04/2018                                                 *
 *===========================================================================*/
]]


player = {
	pos = {
		x = 5.0,
		y = 8.0
	},

	file_name = "player.png",

	health_points = 50
}

enemy = {
	pos = {
		x = 18.0,
		y = 8.0
	},

	file_name = "enemy.png",

	health_points = 200
}