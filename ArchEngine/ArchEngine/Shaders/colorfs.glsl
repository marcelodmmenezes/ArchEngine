/*===========================================================================*
 * Arch Engine - "Testers/colorfs.glsl"                                      *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 26/05/2018                                                 *
 *===========================================================================*/


#version 330 core

uniform vec3 u_color;

out vec4 out_color;

void main() {
	out_color = vec4(u_color, 1.0f);
}
