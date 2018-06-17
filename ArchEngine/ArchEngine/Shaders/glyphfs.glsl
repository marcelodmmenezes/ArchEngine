#version 330 core

in vec2 f_texture_coords;

uniform sampler2D u_texture;
uniform vec3 u_color;

out vec4 out_color;

void main() {
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(u_texture, f_texture_coords).r);
	out_color = vec4(u_color, 1.0f) * sampled;
}