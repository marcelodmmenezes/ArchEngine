#version 330 core

layout (location = 0) in vec4 v_vertex;

uniform mat4 u_projection_matrix;

out vec2 f_texture_coords;

void main() {
	f_texture_coords = v_vertex.zw;
	gl_Position = u_projection_matrix * vec4(v_vertex.xy, 0.0f, 1.0f);
}