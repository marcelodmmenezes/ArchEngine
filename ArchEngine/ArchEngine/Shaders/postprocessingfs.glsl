#version 330 core

in vec2 f_texture_coords;

uniform sampler2D u_texture;

uniform float u_contrast;

out vec4 out_color;

void main() {
	out_color = texture(u_texture, f_texture_coords);
	out_color.rgb = (out_color.rgb - 0.5f) * (1.0 + u_contrast) + 0.5f;
}