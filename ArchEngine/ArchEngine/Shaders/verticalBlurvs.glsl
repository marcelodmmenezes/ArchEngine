#version 330 core

layout (location = 0) in vec4 v_vertex;

uniform float u_target_height;

out vec2 f_blur_texture_coords[11];

void main() {
	gl_Position = vec4(v_vertex.xy, 0.0f, 1.0f);
	vec2 center_texture_coords = vec2(v_vertex.z, 1.0f - v_vertex.w);

	float pixel_size = 1.0f / u_target_height;

	for (int i = -5; i <= 5; i++)
		f_blur_texture_coords[i + 5] = center_texture_coords + vec2(0.0f, pixel_size * i);
}
