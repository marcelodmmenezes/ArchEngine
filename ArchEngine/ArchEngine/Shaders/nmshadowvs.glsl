#version 330 core

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 3
#define NR_SPOT_LIGHTS 1

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture_coords;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;

uniform int u_nr_dir_lights;
uniform int u_nr_point_lights;
uniform int u_nr_spot_lights;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_trn_inv_up_model;
uniform mat4 u_dir_light_space_matrix[NR_DIR_LIGHTS];

out mat3 f_tbn;
out vec3 f_frag_pos;
out vec2 f_texture_coords;
flat out ivec3 f_nr_of_lights;
out vec4 f_frag_pos_dir_light_space[NR_DIR_LIGHTS];

void main() {
	vec3 t = normalize(u_trn_inv_up_model * v_tangent);
	vec3 n = normalize(u_trn_inv_up_model * v_normal);
	vec3 b = normalize(u_trn_inv_up_model * v_bitangent);

	t = normalize(t - n * dot(n, t));

	if (dot(cross(n, t), b) < 0.0f)
		t *= -1.0f;

	f_tbn = transpose(mat3(t, b, n));

	vec4 world_position = u_model_matrix * vec4(v_position, 1.0f);

    f_frag_pos = vec3(world_position);
	f_texture_coords = v_texture_coords;
    f_nr_of_lights = ivec3(u_nr_dir_lights, u_nr_point_lights, u_nr_spot_lights);

	for (int i = 0; i < u_nr_dir_lights; i++)
		f_frag_pos_dir_light_space[i] = u_dir_light_space_matrix[i] * vec4(f_frag_pos, 1.0f);

    gl_Position = u_projection_matrix * u_view_matrix * world_position;
}