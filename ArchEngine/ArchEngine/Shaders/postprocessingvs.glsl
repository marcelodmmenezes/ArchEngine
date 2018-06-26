#version 330 core

layout (location = 0) in vec4 v_vertex;

void main() {
	gl_Position = vec4(v_vertex.xy, 0.0f, 1.0f);
}