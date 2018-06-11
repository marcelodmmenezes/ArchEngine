#version 330 core

uniform vec3 u_color;

out vec4 out_color;

void main() {
	out_color = vec4(u_color, 1.0f);
}