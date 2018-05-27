/*===========================================================================*
 * Arch Engine - "Testers/objvs.glsl"                                        *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 26/05/2018                                                 *
 *===========================================================================*/


#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture_coords;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_trn_inv_up_model;

out vec3 f_normal;
out vec3 f_frag_pos;
out vec2 f_texture_coords;

void main() {
	vec4 world_position = u_model_matrix * vec4(v_position, 1.0f);
	
	f_normal = u_trn_inv_up_model * v_normal;
	f_frag_pos = vec3(world_position);
	f_texture_coords = v_texture_coords;
	
	gl_Position = u_projection_matrix * u_view_matrix * world_position;
}