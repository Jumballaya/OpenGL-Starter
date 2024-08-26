#version 330 core

precision mediump float;

out vec4 outColor;

in vec2 v_uv;

uniform sampler2D u_texture_diffuse_0;

void main() {
	vec4 color = texture(u_texture_diffuse_0, v_uv);
	outColor = color;
}