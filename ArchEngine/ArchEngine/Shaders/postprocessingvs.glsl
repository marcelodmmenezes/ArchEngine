#version 330 core

layout (location = 0) in vec4 v_vertex;

out vec2 f_texture_coords;

void main() {
	f_texture_coords = vec2(v_vertex.z, 1.0f - v_vertex.w);
	gl_Position = vec4(v_vertex.xy, 0.0f, 1.0f);
}