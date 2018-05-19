/*===========================================================================*
 * Arch Engine - "Testers/simplevs.glsl"                                     *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 19/05/2018                                                 *
 *===========================================================================*/


#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 f_color;

void main() {
	f_color = v_normal;
	gl_Position = u_projection * u_view * u_model * vec4(v_position, 1.0f);
}