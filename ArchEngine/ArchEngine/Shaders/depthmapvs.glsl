#version 330 core

layout (location = 0) in vec3 v_position;

uniform mat4 u_light_space_matrix;
uniform mat4 u_model_matrix;

void main() {
	gl_Position = u_light_space_matrix * u_model_matrix * vec4(v_position, 1.0f);
}