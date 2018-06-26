#version 330 core

in vec2 f_blur_texture_coords[11];

uniform sampler2D u_texture;

out vec4 out_color;

void main() {
	out_color = vec4(0.0f);
	out_color += texture(u_texture, f_blur_texture_coords[0]) * 0.0093;
	out_color += texture(u_texture, f_blur_texture_coords[1]) * 0.028002;
	out_color += texture(u_texture, f_blur_texture_coords[2]) * 0.065984;
	out_color += texture(u_texture, f_blur_texture_coords[3]) * 0.121703;
	out_color += texture(u_texture, f_blur_texture_coords[4]) * 0.175713;
	out_color += texture(u_texture, f_blur_texture_coords[5]) * 0.198596;
	out_color += texture(u_texture, f_blur_texture_coords[6]) * 0.175713;
	out_color += texture(u_texture, f_blur_texture_coords[7]) * 0.121703;
	out_color += texture(u_texture, f_blur_texture_coords[8]) * 0.065984;
	out_color += texture(u_texture, f_blur_texture_coords[9]) * 0.028002;
	out_color += texture(u_texture, f_blur_texture_coords[10]) * 0.0093;
}