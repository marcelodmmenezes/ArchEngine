/*===========================================================================*
 * Arch Engine - "Testers/colorvs.glsl"                                      *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 31/05/2018                                                       *
 * Last Modified: 31/05/2018                                                 *
 *===========================================================================*/


#version 330 core

layout (location = 0) in vec3 v_position;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

void main() {
	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(v_position, 1.0f);
}