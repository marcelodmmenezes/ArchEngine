#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

out vec4 g_normal;

void main() {
	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(v_position, 1.0f);
	mat4 normal_matrix = mat4(transpose(inverse(u_model_matrix)));
	g_normal = normalize(u_projection_matrix * u_view_matrix * normal_matrix * vec4(v_normal, 0.0f));
}