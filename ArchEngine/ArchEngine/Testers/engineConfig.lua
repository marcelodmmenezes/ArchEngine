--[[
/*===========================================================================*
 * Arch Engine - "Testers/engineConfig.lua"                                  *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/04/2018                                                       *
 * Last Modified: 25/04/2018                                                 *
 *===========================================================================*/
]]


-- Testing a constant game speed independent of variable FPS game loop
-- Based in: (http://www.koonsolo.com/news/dewitters-gameloop/)
ticks_per_second = 25
skip_ticks = 1000 / ticks_per_second
max_frameskip = 5