#version 330 core

in vec2 f_texture_coords;

uniform sampler2D u_texture;

out vec4 out_color;

void main() {
	out_color = texture(u_texture, f_texture_coords).rgba;
}