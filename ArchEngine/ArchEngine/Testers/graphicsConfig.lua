--[[
/*===========================================================================*
 * Arch Engine - "Testers/graphicsConfig.lua"                                *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 26/05/2018                                                       *
 * Last Modified: 25/06/2018                                                 *
 *===========================================================================*/
]]


depth_test = true
face_culling = true
blending = true

-- Post processing shaders
m_horizontal_gb_shadervs = "../../ArchEngine/Shaders/horizontalBlurvs.glsl"
m_horizontal_gb_shaderfs = "../../ArchEngine/Shaders/gaussianBlurfs.glsl"

m_vertical_gb_shadervs = "../../ArchEngine/Shaders/verticalBlurvs.glsl"
m_vertical_gb_shaderfs = "../../ArchEngine/Shaders/gaussianBlurfs.glsl"

m_bright_shadervs = "../../ArchEngine/Shaders/postprocessingvs.glsl"
m_bright_shaderfs = "../../ArchEngine/Shaders/brightFilterfs.glsl"

m_combine_shadervs = "../../ArchEngine/Shaders/postprocessingvs.glsl"
m_combine_shaderfs = "../../ArchEngine/Shaders/combinefs.glsl"

m_pp_shadervs = "../../ArchEngine/Shaders/postprocessingvs.glsl"
m_pp_shaderfs = "../../ArchEngine/Shaders/postprocessingfs.glsl"
-------------------------

-- Allocation hint
n_shaders = 25
------------------

--clear_color = { 0.05, 0.08, 0.07, 1.0 }
clear_color = { 1.0, 1.0, 1.0, 1.0 }
--clear_color = { 0.0, 0.0, 0.0, 1.0 }

view_port = { 0, 0, 800, 600 }
--view_port = { 0, 0, 1920, 1080 }

fov = 45.0

active_camera = -1