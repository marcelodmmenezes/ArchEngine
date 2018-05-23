/*===========================================================================*
 * Arch Engine - "Testers/objfs.glsl"                                        *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 20/05/2018                                                       *
 * Last Modified: 22/05/2018                                                 *
 *===========================================================================*/


#version 330 core

in vec3 f_normal;
in vec3 f_frag_pos;
in vec2 f_texture_coords;

uniform vec3 u_view_pos;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;

out vec4 f_frag_color;

void main() {
	vec3 norm = normalize(f_normal);
	vec3 view_dir = normalize(u_view_pos - f_frag_pos);

	vec3 diffuse_tex = vec3(texture(u_texture_0, f_texture_coords));
	vec3 specular_tex = vec3(texture(u_texture_1, f_texture_coords));
	//diffuse_tex = vec3(0.4f, 0.3f, 0.2f);
	//specular_tex = vec3(1.0f, 1.0f, 1.0f);

	vec3 result = 0.1f * diffuse_tex;
	
	vec3 light_dir = normalize(vec3(1.0f, 1.0f, 1.0f));

	float diff = max(dot(norm, light_dir), 0.0f);
	
    vec3 halfway_dir = normalize(light_dir + view_dir);  
    float spec = pow(max(dot(norm, halfway_dir), 0.0), 64.0f);
	/*
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 32.0f);
*/
	vec3 ambient = vec3(0.2f, 0.2f, 0.2f) * diffuse_tex;
	vec3 diffuse = vec3(0.5f, 0.5f, 0.5f) * diff * diffuse_tex;
	vec3 specular = vec3(0.7f, 0.7f, 0.7f) * spec * specular_tex;

	result += ambient + diffuse + specular;

    f_frag_color = vec4(pow(result, vec3(1.0f / 1.1f)), 1.0f);
}