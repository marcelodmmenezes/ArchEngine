#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture_coords;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat3 u_trn_inv_up_model;

uniform vec3 u_view_pos;

out vec3 f_frag_pos;
out vec2 f_texture_coords;
out vec3 f_tangent_light_pos;
out vec3 f_tangent_view_pos;
out vec3 f_tangent_frag_pos;

void main() {
	f_frag_pos = vec3(u_model * vec4(v_position, 1.0f));
	f_texture_coords = v_texture_coords;
	
    vec3 t = normalize(u_trn_inv_up_model * v_tangent);
    vec3 b = normalize(u_trn_inv_up_model * v_bitangent);
    vec3 n = normalize(u_trn_inv_up_model * v_normal);
    //t = normalize(t - dot(t, n) * n);

    if (dot(cross(n, t), b) < 0.0)
    	t *= -1;
    
    mat3 tbn = transpose(mat3(t, b, n));    
    f_tangent_light_pos = tbn * vec3(50.0f, 30.0f, 0.0f);
    f_tangent_view_pos  = tbn * u_view_pos;
    f_tangent_frag_pos  = tbn * f_frag_pos;

	gl_Position = u_projection * u_view * u_model * vec4(v_position, 1.0f);
}