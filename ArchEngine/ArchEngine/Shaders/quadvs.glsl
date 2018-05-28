#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 2) in vec2 v_texture_coords;

uniform mat4 u_model_matrix;

out vec2 f_texture_coords;

void main() {
	gl_Position = u_model_matrix * vec4(v_position, 1.0f);
	f_texture_coords = v_texture_coords;
}