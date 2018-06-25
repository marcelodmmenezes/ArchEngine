#version 330 core

in vec3 f_texture_coords;

uniform samplerCube u_cube_map;

uniform bool u_fog;
uniform vec3 u_fog_color;
uniform float u_lower_limit;
uniform float u_upper_limit;

out vec4 out_color;

void main() {
	if (u_fog) {
		vec4 tex = texture(u_cube_map, f_texture_coords);
		float factor = (f_texture_coords.y - u_lower_limit / u_upper_limit - u_lower_limit);
		factor = clamp(factor, 0.0f, 1.0f);
		out_color = mix(vec4(u_fog_color, 1.0f), tex, factor);
	}
	else out_color = texture(u_cube_map, f_texture_coords);
}