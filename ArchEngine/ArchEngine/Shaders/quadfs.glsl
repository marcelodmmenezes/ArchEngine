#version 330 core

in vec2 f_texture_coords;

uniform sampler2D u_texture;
uniform vec4 u_color;

uniform bool u_has_texture;
uniform bool u_has_color;

out vec4 out_color;

void main() {
	vec4 result = vec4(0.0f);

	if (u_has_texture)
		result += texture(u_texture, f_texture_coords);

	if (u_has_color)
		result += u_color;

	out_color = result;
	out_color = vec4(0.18f, 0.27f, 0.27f, 0.5f);
}