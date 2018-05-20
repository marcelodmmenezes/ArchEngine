/*===========================================================================*
 * Arch Engine - "Testers/objvs.glsl"                                        *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 20/05/2018                                                       *
 * Last Modified: 20/05/2018                                                 *
 *===========================================================================*/


#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture_coords;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_trn_inv_up_model;

out vec3 f_normal;
out vec3 f_frag_pos;
out vec2 f_texture_coords;

void main() {
	vec4 world_position = u_model * vec4(v_position, 1.0f);

	gl_Position = u_projection * u_view * world_position;

	f_normal = u_trn_inv_up_model * v_normal;
	f_frag_pos = vec3(world_position);

	f_texture_coords = vec2(v_texture_coords.x, 1.0f - v_texture_coords.y);;
}