#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;

out vec3 f_color;

void main() {
	f_color = v_color;
	gl_Position = u_projection_matrix * u_view_matrix * vec4(v_position, 1.0f);
}