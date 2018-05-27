/*===========================================================================*
 * Arch Engine - "Testers/objfs.glsl"                                        *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 26/05/2018                                                 *
 *===========================================================================*/


 #version 330 core

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

#define NR_DIR_LIGHTS 1
#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

in vec3 f_normal;
in vec3 f_frag_pos;
in vec2 f_texture_coords;

uniform vec3 u_view_pos;
uniform sampler2D u_texture_diffuse;
uniform sampler2D u_texture_specular;

uniform int u_nr_dir_lights;
uniform int u_nr_point_lights;
uniform int u_nr_spot_lights;

uniform DirLight u_dir_lights[NR_DIR_LIGHTS];
uniform PointLight u_point_lights[NR_POINT_LIGHTS];
uniform SpotLight u_spot_lights[NR_SPOT_LIGHTS];

out vec4 out_color;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text);

void main() {
	vec3 normal = normalize(f_normal);
	vec3 view_dir = normalize(u_view_pos - f_frag_pos);

	vec3 diffuse_tex = vec3(texture(u_texture_diffuse, f_texture_coords));
	vec3 specular_tex = vec3(texture(u_texture_specular, f_texture_coords));

	//------------------------------------------------------------------------------------ Light calculations
	vec3 result = 0.15f * diffuse_tex;
	
	for(int i = 0; i < u_nr_dir_lights; i++)
		result += calcDirLight(u_dir_lights[i], normal, view_dir, diffuse_tex, specular_tex);

	for(int i = 0; i < u_nr_point_lights; i++)
		result += calcPointLight(u_point_lights[i], normal, view_dir, diffuse_tex, specular_tex);
		
	for(int i = 0; i < u_nr_spot_lights; i++)
		result += calcSpotLight(u_spot_lights[i], normal, view_dir, diffuse_tex, specular_tex);
	//-------------------------------------------------------------------------------------------------------

    out_color = vec4(pow(result, vec3(1.0f / 1.1f)), 1.0f);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text) {
	vec3 light_dir = normalize(-light.direction);

	float diff = max(dot(normal, light_dir), 0.0f);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), light.shininess);

	vec3 ambient = light.ambient * diff_text;
	vec3 diffuse = light.diffuse * diff * diff_text;
	vec3 specular = light.specular * spec * spec_text;

	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text) {
	vec3 pos_minus_frag = light.position - f_frag_pos;
	vec3 light_dir = normalize(pos_minus_frag);

	float diff = max(dot(normal, light_dir), 0.0f);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), light.shininess);

	float dist = length(pos_minus_frag);
	float att = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 ambient = light.ambient * att * diff_text;
	vec3 diffuse = light.diffuse * att * diff * diff_text;
	vec3 specular = light.specular * att * spec * spec_text;

	return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text) {
	vec3 pos_minus_frag = light.position - f_frag_pos;
	vec3 light_dir = normalize(pos_minus_frag);

	float theta = dot(light_dir, normalize(-light.direction));
	float epsilon = light.inner_cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0f, 1.0f);

	float diff = max(dot(normal, light_dir), 0.0f);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), light.shininess);

	float dist = length(pos_minus_frag);
	float att = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 ambient = light.ambient * att * diff_text;
	vec3 diffuse = light.diffuse * att * intensity * diff * diff_text;
	vec3 specular = light.specular * att * intensity * spec * spec_text;

	return ambient + diffuse + specular;
}