/*===========================================================================*
 * Arch Engine - "Testers/simplefs.glsl"                                     *
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

in vec3 f_color;

out vec4 color;

void main() {
	color = vec4((f_color / 2) + 0.5f, 1.0f);
}
