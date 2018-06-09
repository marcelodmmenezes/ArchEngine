#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_trn_inv_up_model;

out vec3 g_normal;

void main() {
	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(v_position, 1.0f);
	g_normal = normalize(vec3(u_projection_matrix * vec4(u_trn_inv_up_model * v_normal, 0.0f)));
}