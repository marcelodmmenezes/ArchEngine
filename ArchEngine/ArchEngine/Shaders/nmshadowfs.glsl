#version 330 core

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

struct DirLight {
	vec3 direction;
	
	float shininess;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	
	float shininess;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	
	float shininess;

	float inner_cut_off;
	float outer_cut_off;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in mat3 f_tbn;
in vec3 f_frag_pos;
in vec2 f_texture_coords;
flat in ivec3 f_nr_of_lights;
in vec4 f_frag_pos_dir_light_space[NR_DIR_LIGHTS];

uniform vec3 u_view_pos;

uniform float u_far_plane;

uniform sampler2D u_texture_diffuse;
uniform sampler2D u_texture_specular;
uniform sampler2D u_texture_normals;
uniform sampler2D u_dir_shadow_map[NR_DIR_LIGHTS];
uniform samplerCube u_point_shadow_map[NR_POINT_LIGHTS];

uniform DirLight u_dir_lights[NR_DIR_LIGHTS];
uniform PointLight u_point_lights[NR_POINT_LIGHTS];
uniform SpotLight u_spot_lights[NR_SPOT_LIGHTS];

out vec4 out_color;

vec3 g_tangent_frag_pos;

vec3 g_grid_sampling_disk[20] = vec3[] (
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir, vec3 diff_text, vec3 spec_text, int id);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 diff_text, vec3 spec_text, int id);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, vec3 diff_text, vec3 spec_text);

float calcDirShadows(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir, int light_index);
float calcPointShadows(int light_index);

void main() {
	vec3 normal = normalize(texture(u_texture_normals, f_texture_coords).rgb * 2.0f - 1.0f);
	g_tangent_frag_pos = f_tbn * f_frag_pos;

	vec3 view_dir = normalize(f_tbn * u_view_pos - g_tangent_frag_pos);

	vec3 diffuse_tex = vec3(texture(u_texture_diffuse, f_texture_coords));
	vec3 specular_tex = vec3(texture(u_texture_specular, f_texture_coords));

	//------------------------------------------------------------------------------------ Light calculations
	vec3 ambient = 0.15f * diffuse_tex;
	vec3 result = vec3(0.0f);
	
	for(int i = 0; i < f_nr_of_lights[0]; i++)
		result += calcDirLight(u_dir_lights[i], normal, view_dir, diffuse_tex, specular_tex, i);

	for(int i = 0; i < f_nr_of_lights[1]; i++)
		result += calcPointLight(u_point_lights[i], normal, view_dir, diffuse_tex, specular_tex, i);
			
	for(int i = 0; i < f_nr_of_lights[2]; i++)
		result += calcSpotLight(u_spot_lights[i], normal, view_dir, diffuse_tex, specular_tex);
	//-------------------------------------------------------------------------------------------------------

    out_color.rgb = pow(ambient + result, vec3(1.0f / 1.2f));
	out_color.a = 1.0f;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir, vec3 diff_text, vec3 spec_text, int id) {
	vec3 light_dir = normalize(f_tbn * (-light.direction));

	float diff = max(dot(normal, light_dir), 0.0f);

    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 2 * light.shininess);

	vec3 diffuse = light.diffuse * diff * diff_text;
	vec3 specular = light.specular * spec * spec_text;

	return (1.0f - calcDirShadows(f_frag_pos_dir_light_space[id], normal, light_dir, id)) * (diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 diff_text, vec3 spec_text, int id) {
	vec3 pos_minus_frag = f_tbn * light.position - g_tangent_frag_pos;
	vec3 light_dir = normalize(pos_minus_frag);

	float diff = max(dot(normal, light_dir), 0.0f);

    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 2 * light.shininess);

	float dist = length(pos_minus_frag);
	float att = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 diffuse = light.diffuse * att * diff * diff_text;
	vec3 specular = light.specular * att * spec * spec_text;

	return (1.0f - calcPointShadows(id)) * (diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, vec3 diff_text, vec3 spec_text) {
	vec3 pos_minus_frag = f_tbn * light.position - g_tangent_frag_pos;
	vec3 light_dir = normalize(pos_minus_frag);

	float theta = dot(light_dir, normalize(f_tbn * (-light.direction)));
	float epsilon = light.inner_cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0f, 1.0f);

	float diff = max(dot(normal, light_dir), 0.0f);

    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 2 * light.shininess);

	float dist = length(pos_minus_frag);
	float att = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 diffuse = light.diffuse * att * intensity * diff * diff_text;
	vec3 specular = light.specular * att * intensity * spec * spec_text;

	return diffuse + specular;
}

float calcDirShadows(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir, int light_index) {
	vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
	proj_coords = proj_coords * 0.5f + 0.5f;

	float current_depth = proj_coords.z;

	float bias = max(0.001f * (1.0f - dot(normal, light_dir)), 0.0001f);

	if (proj_coords.z > 0.9f)
		return 0.0f;

	//------------------------------------------------------------------------------------------------------------- PCF
	float shadow = 0.0f;

	vec2 texel_size = 1.0f / textureSize(u_dir_shadow_map[0], 0);

	for(int x = -2; x <= 2; x++) {
		for(int y = -2; y <= 2; y++) {
    	// TODO: find how to access sampler2D array dinamically
			float pcf_depth = texture(u_dir_shadow_map[0], proj_coords.xy + vec2(x, y) * texel_size).r; 
			shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;        
		}    
	}

	shadow /= 25.0f;
	//-----------------------------------------------------------------------------------------------------------------

	return shadow;
}

float calcPointShadows(int light_index) {
	vec3 frag_to_light = f_frag_pos - u_point_lights[light_index].position;
	float current_depth = length(frag_to_light);
    float shadow = 0.0;
    float bias = 0.00015f;
    int samples = 20;
    float view_distance = length(u_view_pos - f_frag_pos);

	float disk_radius = (1.0f + (view_distance / u_far_plane)) / 25.0f;

    for(int i = 0; i < samples; i++) {
    	// TODO: find how to access samplerCube array dinamically
    	float closest_depth = texture(u_point_shadow_map[0], frag_to_light + g_grid_sampling_disk[i] * disk_radius).r;
		closest_depth *= u_far_plane;
        if(current_depth - bias > closest_depth)
            shadow += 1.0f;
    }

    shadow /= float(samples);

    return shadow;
}