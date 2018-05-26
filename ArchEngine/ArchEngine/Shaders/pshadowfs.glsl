#version 330 core

in vec3 f_frag_pos;
in vec2 f_texture_coords;
in vec3 f_tangent_light_pos;
in vec3 f_tangent_view_pos;
in vec3 f_tangent_frag_pos;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform samplerCube u_texture_3;

const vec3 point_light_position = vec3(30.0f, 50.0f, 0.0f);

vec3 g_grid_sampling_disk[20] = vec3[] (
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

out vec4 color;

float calcShadows() {
	vec3 frag_to_light = f_frag_pos - point_light_position;
	float current_depth = length(frag_to_light);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float view_distance = length(f_tangent_view_pos - f_tangent_frag_pos);
    float disk_radius = (1.0 + (view_distance / 50.0f)) / 25.0;
    
    for(int i = 0; i < samples; i++) {
        float closest_depth = texture(u_texture_3, frag_to_light + g_grid_sampling_disk[i] * disk_radius).r;
        closest_depth *= 50.0f;
        if(current_depth - bias > closest_depth)
            shadow += 1.0;
    }

    shadow /= float(samples);
    return shadow;
}

void main() {
    vec3 normal = texture(u_texture_2, f_texture_coords).rgb;

    normal = normalize(normal * 2.0f - 1.0f);

    vec3 diffuse_tex = texture(u_texture_0, f_texture_coords).rgb;
    vec3 specular_tex = texture(u_texture_1, f_texture_coords).rgb;

    vec3 ambient = 0.1f * diffuse_tex;

    vec3 light_dir = normalize(f_tangent_light_pos - f_tangent_frag_pos);
    float diff = max(dot(light_dir, normal), 0.0f);
    vec3 diffuse = diff * diffuse_tex;

    vec3 view_dir = normalize(f_tangent_view_pos - f_tangent_frag_pos);
    vec3 halfwayDir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
    vec3 specular = vec3(1.0f) * spec * specular_tex;
	
	//----------------------------------------------------------------------------------- Shadow calculations
	float shadow = calcShadows();
	vec3 lighting = ambient + ((1.0f - shadow) * diffuse + specular);
	//-------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------- Gamma correction
    float gamma = 1.2f;
    color.rgb = pow(lighting, vec3(1.0f/gamma));
    //-------------------------------------------------------------------------------------------------------
}