#version 330 core

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture_coords;
layout (location = 3) in ivec4 v_bone_ids;
layout (location = 4) in vec4 v_bone_weights;

const int MAX_BONES = 50;
uniform mat4 u_bones[MAX_BONES];

uniform int u_nr_dir_lights;
uniform int u_nr_point_lights;
uniform int u_nr_spot_lights;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
uniform mat4 u_dir_light_space_matrix[NR_DIR_LIGHTS];

out vec3 f_normal;
out vec3 f_frag_pos;
out vec2 f_texture_coords;
flat out ivec3 f_nr_of_lights;
out vec4 f_frag_pos_dir_light_space[NR_DIR_LIGHTS];

void main() {
    mat4 bone_transform = u_bones[v_bone_ids[0]] * v_bone_weights[0];
    bone_transform += u_bones[v_bone_ids[1]] * v_bone_weights[1];
    bone_transform += u_bones[v_bone_ids[2]] * v_bone_weights[2];
    bone_transform += u_bones[v_bone_ids[3]] * v_bone_weights[3];

	vec4 world_position = u_model_matrix * bone_transform * vec4(v_position, 1.0f);

	f_normal = mat3(bone_transform) * v_normal;
    f_frag_pos = vec3(world_position);   
    f_texture_coords = v_texture_coords;
    f_nr_of_lights = ivec3(u_nr_dir_lights, u_nr_point_lights, u_nr_spot_lights);

	for (int i = 0; i < u_nr_dir_lights; i++)
		f_frag_pos_dir_light_space[i] = u_dir_light_space_matrix[i] * vec4(f_frag_pos, 1.0f);

    gl_Position = u_projection_matrix * u_view_matrix * world_position;
}