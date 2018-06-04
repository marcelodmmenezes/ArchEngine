#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 3) in ivec4 v_bone_ids;
layout (location = 4) in vec4 v_bone_weights;

const int MAX_BONES = 100;
uniform mat4 u_bones[MAX_BONES];

uniform mat4 u_model_matrix;
uniform bool u_rigged;

void main() {
	if (u_rigged) {
		mat4 bone_transform = u_bones[v_bone_ids[0]] * v_bone_weights[0];
		bone_transform += u_bones[v_bone_ids[1]] * v_bone_weights[1];
		bone_transform += u_bones[v_bone_ids[2]] * v_bone_weights[2];
		bone_transform += u_bones[v_bone_ids[3]] * v_bone_weights[3];

		gl_Position = u_model_matrix * bone_transform * vec4(v_position, 1.0f);
	}
	else
		gl_Position = u_model_matrix * vec4(v_position, 1.0f);
}