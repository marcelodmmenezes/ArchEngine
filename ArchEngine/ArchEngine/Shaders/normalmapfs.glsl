#version 330 core

in vec3 f_frag_pos;
in vec2 f_texture_coords;
in vec3 f_tangent_light_pos;
in vec3 f_tangent_view_pos;
in vec3 f_tangent_frag_pos;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;

out vec4 color;

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
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
    vec3 specular = vec3(1.0f) * spec * specular_tex;
    
    color = vec4(ambient + diffuse + specular, 1.0f);

    //-------------------------------------------------------------------------------------- Gamma correction
    float gamma = 1.2f;
    color.rgb = pow(color.rgb, vec3(1.0f/gamma));
    //-------------------------------------------------------------------------------------------------------
}