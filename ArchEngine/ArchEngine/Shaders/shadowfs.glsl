#version 330 core

in vec3 f_normal;
in vec3 f_frag_pos;
in vec2 f_texture_coords;
in vec3 f_tangent_light_pos;
in vec3 f_tangent_view_pos;
in vec3 f_tangent_frag_pos;
in vec4 f_frag_pos_light_space;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2; // normal_map
uniform sampler2D u_texture_3; // shadow_map

out vec4 color;


float calcShadows(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir) {
	vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
	proj_coords = proj_coords * 0.5f + 0.5f;

	float closest_depth = texture(u_texture_3, proj_coords.xy).r;
	float current_depth = proj_coords.z;

	float bias = max(0.03f * (1.0f - dot(normal, light_dir)), 0.003f);

	if (proj_coords.z > 0.9f)
		return 0.0f;

	return current_depth - bias > closest_depth ? 1.0f : 0.0f;
	//return current_depth > closest_depth ? 1.0f : 0.0f;
}


void main() {
    vec3 normal = texture(u_texture_2, f_texture_coords).rgb;

    normal = normalize(normal * 2.0f - 1.0f);

    vec3 diffuse_tex = texture(u_texture_0, f_texture_coords).rgb;
    vec3 specular_tex = texture(u_texture_1, f_texture_coords).rgb;
	
	//---------------------------------------------------------------------------------------------- Lighting
    vec4 ambient = vec4(0.1f * diffuse_tex, 1.0f);

    vec3 light_dir = normalize(f_tangent_light_pos - f_tangent_frag_pos);
    float diff = max(dot(light_dir, normal), 0.0f);
    vec3 diffuse = diff * diffuse_tex;

    vec3 view_dir = normalize(f_tangent_view_pos - f_tangent_frag_pos);
    vec3 halfwayDir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
    vec3 specular = vec3(1.0f) * spec * specular_tex;
    
    vec4 result = vec4(diffuse + specular, 1.0f);
	//-------------------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------- Shadow calculations
	float shadow = calcShadows(f_frag_pos_light_space, f_normal, vec3(60.0f, 150.0f, 0.0f));
	color = ambient + ((1.0f - shadow) * result);
	//-------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------- Gamma correction
    float gamma = 1.2f;
    color.rgb = pow(color.rgb, vec3(1.0f/gamma));
    //-------------------------------------------------------------------------------------------------------
}