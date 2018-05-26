#version 330 core

in vec3 f_texture_coords;

uniform samplerCube u_texture;

out vec4 color;

void main() {
	color = vec4(vec3(texture(u_texture, f_texture_coords).r), 1.0f);
}