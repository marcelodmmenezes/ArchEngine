#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture_coords;

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

uniform int u_nr_dir_lights;
uniform int u_nr_point_lights;
uniform int u_nr_spot_lights;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_trn_inv_up_model;
uniform mat4 u_dir_light_space_matrix[NR_DIR_LIGHTS];
uniform mat4 u_point_light_space_matrix[NR_POINT_LIGHTS];
uniform mat4 u_spot_light_space_matrix[NR_SPOT_LIGHTS];

out vec3 f_normal;
out vec3 f_frag_pos;
out vec2 f_texture_coords;
flat out ivec3 f_nr_of_lights;
out vec4 f_frag_pos_dir_light_space[NR_DIR_LIGHTS];
out vec4 f_frag_pos_point_light_space[NR_POINT_LIGHTS];
out vec4 f_frag_pos_spot_light_space[NR_SPOT_LIGHTS];

void main() {
	f_normal = u_trn_inv_up_model * v_normal;
    f_frag_pos = vec3(u_model_matrix * vec4(v_position, 1.0f));   
    f_texture_coords = v_texture_coords;
    f_nr_of_lights = ivec3(u_nr_dir_lights, u_nr_point_lights, u_nr_spot_lights);

	for (int i = 0; i < u_nr_dir_lights; i++)
		f_frag_pos_dir_light_space[i] = u_dir_light_space_matrix[i] * vec4(f_frag_pos, 1.0f);

	for (int i = 0; i < u_nr_point_lights; i++)
		f_frag_pos_point_light_space[i] = u_point_light_space_matrix[i] * vec4(f_frag_pos, 1.0f);

	for (int i = 0; i < u_nr_spot_lights; i++)
		f_frag_pos_spot_light_space[i] = u_spot_light_space_matrix[i] * vec4(f_frag_pos, 1.0f);

    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(v_position, 1.0f);
}