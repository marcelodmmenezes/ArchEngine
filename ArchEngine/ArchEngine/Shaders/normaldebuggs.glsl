#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 g_normal[];

uniform float u_magnitude;

void generateLine(int id) {
	gl_Position = gl_in[id].gl_Position;
	EmitVertex();
	gl_Position = gl_in[id].gl_Position + vec4(g_normal[id], 0.0f) * u_magnitude;
	EmitVertex();
	EndPrimitive();
}

void main() {
	generateLine(0);
	generateLine(1);
	generateLine(2);
}