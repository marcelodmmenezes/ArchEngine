#version 330 core

in vec4 f_frag_pos;

uniform vec3 u_light_pos;
uniform float u_far_plane;

void main() {
    gl_FragDepth = length(f_frag_pos.xyz - u_light_pos) / u_far_plane;
}