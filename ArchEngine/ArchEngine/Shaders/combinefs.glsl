#version 330 core

in vec2 f_texture_coords;

uniform sampler2D u_color_texture;
uniform sampler2D u_highlight_texture;
uniform float u_glow_factor;

out vec4 out_color;

void main() {
	vec4 scene_color = texture(u_color_texture, f_texture_coords);
	vec4 hightlight_color = texture(u_highlight_texture, f_texture_coords);
	out_color = scene_color + hightlight_color * u_glow_factor;
}