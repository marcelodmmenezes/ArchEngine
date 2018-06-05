--[[
/*===========================================================================*
 * Arch Engine - "Testers/physicsConfig.lua"                                 *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 04/06/2018                                                       *
 * Last Modified: 05/06/2018                                                 *
 *===========================================================================*/
]]


world = {
	world_type = 0, -- WORLD_DISCRETE_DYNAMICS
	collision_config = 0, -- DEFAULT_COLLISION_CONFIGURATION
	broadphase = 1, -- DBVT_BROADPHASE
	solver = 0 -- SEQUENTIAL_IMPULSE_SOLVER
}