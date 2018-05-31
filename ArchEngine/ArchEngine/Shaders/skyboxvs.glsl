#version 330 core

layout (location = 0) in vec3 v_position;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 f_texture_coords;

void main() {
	gl_Position = (u_projection * u_view * vec4(v_position, 1.0f)).xyww;
	f_texture_coords = v_position;
}