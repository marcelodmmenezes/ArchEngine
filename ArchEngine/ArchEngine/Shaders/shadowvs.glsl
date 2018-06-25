#version 330 core

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 3
#define NR_SPOT_LIGHTS 1

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture_coords;

uniform int u_nr_dir_lights;
uniform int u_nr_point_lights;
uniform int u_nr_spot_lights;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_trn_inv_up_model;
uniform mat4 u_dir_light_space_matrix[NR_DIR_LIGHTS];

uniform float u_fog_density; // Fog
uniform float u_gradient; // Fog

out vec3 f_normal;
out vec3 f_frag_pos;
out vec2 f_texture_coords;
flat out ivec3 f_nr_of_lights;
out vec4 f_frag_pos_dir_light_space[NR_DIR_LIGHTS];

out float f_visibility; // Fog

void main() {
	// Fog
	vec4 position_relative_to_camera = u_view_matrix * u_model_matrix * vec4(v_position, 1.0f);
	float dist = length(position_relative_to_camera.xyz);
	f_visibility = exp(-pow((dist * u_fog_density), u_gradient));
	f_visibility = clamp(f_visibility, 0.0f, 1.0f);
	//----

	f_normal = u_trn_inv_up_model * v_normal;
    f_frag_pos = vec3(u_model_matrix * vec4(v_position, 1.0f));   
    f_texture_coords = v_texture_coords;
    f_nr_of_lights = ivec3(u_nr_dir_lights, u_nr_point_lights, u_nr_spot_lights);

	for (int i = 0; i < u_nr_dir_lights; i++)
		f_frag_pos_dir_light_space[i] = u_dir_light_space_matrix[i] * vec4(f_frag_pos, 1.0f);

    gl_Position = u_projection_matrix * position_relative_to_camera;

}