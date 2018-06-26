#version 330 core

in vec2 f_texture_coords;

uniform sampler2D u_texture;

out vec4 out_color;

void main() {
	vec4 color = texture(u_texture, f_texture_coords);
	float brightness = 2 * ((color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722));
	out_color = vec4(color.r * brightness, color.g * brightness, color.b * brightness, 1.0f);
}