/*===========================================================================*
 * Arch Engine - "Testers/objfs.glsl"                                        *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 13/05/2018                                                 *
 *===========================================================================*/


 #version 330 core

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

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
	float cut_off;
	float outer_cut_off;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4

layout (location = 0) out vec4 f_frag_color;
layout (location = 1) out vec4 f_bright_color;

in vec3 f_normal;
in vec3 f_frag_pos;
in vec2 f_texture_coords;

uniform vec3 u_view_pos;
uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_diffuse2;
uniform sampler2D u_texture_diffuse3;
uniform sampler2D u_texture_specular1;
uniform sampler2D u_texture_specular2;
uniform float u_shineness;

uniform DirLight u_dir_light;
uniform PointLight u_point_lights[NR_POINT_LIGHTS];
uniform SpotLight u_spot_light;

uniform bool u_calc_dl;
uniform bool u_calc_pl[NR_POINT_LIGHTS];
uniform bool u_calc_sl;

uniform bool u_discard_transparent_textures;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text);

void main() {
	if(u_discard_transparent_textures && texture(u_texture_diffuse1, f_texture_coords).a < 0.1)
		discard;

	vec3 norm = normalize(f_normal);
	vec3 view_dir = normalize(u_view_pos - f_frag_pos);

	vec3 diffuse_tex = vec3(texture(u_texture_diffuse1, f_texture_coords));
	vec3 specular_tex = vec3(texture(u_texture_specular1, f_texture_coords));

	//------------------------------------------------------------------------------------ Light calculations
	vec3 result = 0.1f * diffuse_tex;

	if(u_calc_dl)
		result += calcDirLight(u_dir_light, norm, view_dir, diffuse_tex, specular_tex);

	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		if(u_calc_pl[i])
			result += calcPointLight(u_point_lights[i], norm, view_dir, diffuse_tex, specular_tex);

	if(u_calc_sl)
		result += calcSpotLight(u_spot_light, norm, view_dir, diffuse_tex, specular_tex);
	//-------------------------------------------------------------------------------------------------------

	f_frag_color = vec4(result, 1.0f);


    const float gamma = 1.1f;
    f_frag_color = vec4(pow(result, vec3(1.0f / gamma)), 1.0f);

    float brightness = dot(f_frag_color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 0.8f)
        f_bright_color = vec4(f_frag_color.rgb, 1.0f);
    else
        f_bright_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 view_dir, inout vec3 diff_text, inout vec3 spec_text) {
	vec3 light_dir = normalize(-light.direction);

	float diff = max(dot(normal, light_dir), 0.0f);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), u_shineness);

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
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), u_shineness);

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
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0f, 1.0f);

	float diff = max(dot(normal, light_dir), 0.0f);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), u_shineness);

	float dist = length(pos_minus_frag);
	float att = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 ambient = light.ambient * att * diff_text;
	vec3 diffuse = light.diffuse * att * intensity * diff * diff_text;
	vec3 specular = light.specular * att * intensity * spec * spec_text;

	return ambient + diffuse + specular;
}