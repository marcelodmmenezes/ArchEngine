#version 330 core

in vec3 f_texture_coords;

uniform samplerCube u_cube_map;

out vec4 color;

void main() {
	color = texture(u_cube_map, f_texture_coords);
}