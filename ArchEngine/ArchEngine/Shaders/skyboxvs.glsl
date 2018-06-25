#version 330 core

layout (location = 0) in vec3 v_position;

uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

out vec3 f_texture_coords;

void main() {
	gl_Position = (u_projection_matrix * u_view_matrix * vec4(v_position, 1.0f)).xyww;
	f_texture_coords = v_position;
}