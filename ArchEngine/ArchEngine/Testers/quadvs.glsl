#version 330 core

layout (location = 0) v_position;
layout (location = 1) v_texture_coords;

out vec2 f_texture_coords;

void main() {
	gl_Position = vec4(v_position, 1.0f);
	f_texture_coords = v_texture_coords;
}