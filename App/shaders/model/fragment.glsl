#version 460 core

precision mediump float;

out vec4 outColor;

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_world_pos;

layout(std140, binding = 0) uniform camera {
	uniform mat4 view_matrix;
	uniform mat4 projection_matrix;
	uniform vec4 camera_position;
};
layout (binding = 0) uniform sampler2D texture0;
layout (binding = 1) uniform samplerCube texture1;

void main() {
	vec3 n = normalize(v_normal);
	vec3 v = normalize(camera_position.xyz - v_world_pos);
	vec3 reflection = -normalize(reflect(v, n));

	float eta = 1.00 / 1.31; // ice
	vec3 refraction = -normalize(refract(v, n, eta));

	const float R0 = ((1.0-eta) * (1.0-eta)) / ((1.0+eta) * (1.0+eta));
	const float Rtheta = R0 + (1.0 - R0) * pow((1.0 - dot(-v, n)), 5.0);

	vec4 albedo = texture(texture0, v_uv);
	vec4 color = albedo;
	vec4 colorRefl = texture(texture1, reflection);
	vec4 colorRefr = texture(texture1, refraction);
	color = color * mix(colorRefl, colorRefr, Rtheta);
	outColor = color;
}
