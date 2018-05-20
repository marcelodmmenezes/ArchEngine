--[[
/*===========================================================================*
 * Arch Engine - "Testers/windowConfig.lua"                                  *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/04/2018                                                       *
 * Last Modified: 18/05/2018                                                 *
 *===========================================================================*/
]]


title = "ArchEngine"

-- Window position and size
pos_x = -1 -- -1 for SDL_WINDOWPOS_CENTERED
pos_y = -1 -- -1 for SDL_WINDOWPOS_CENTERED
width = 800
height = 600

-- Color bits
r_size = 8
g_size = 8
b_size = 8
a_size = 8

-- Buffers size
depth_size = 8
stencil_size = 8

vsync = false -- Just a hint, the driver does what it wants
fullscreen = false

-- Antialiasing
ms_n_buffers = 1
ms_n_samples = 4

-- Supported OpenGL context
context_major_version = 3
context_minor_version = 3

-- SDL Config
sdl_flags = { "SDL_WINDOW_OPENGL", "SDL_WINDOW_RESIZABLE" }

-- Mouse capture
capture_mouse = true

-- Window hot reload
hot_reload = true