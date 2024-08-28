#version 460 core

layout(location=0) in vec4 a_position;
layout(location=1) in vec3 a_normals;
layout(location=2) in vec2 a_uv;

out vec2 v_uv;
out vec3 v_normal;
out vec3 v_world_pos;

layout(std140, binding = 0) uniform camera {
	uniform mat4 view_matrix;
	uniform mat4 projection_matrix;
	uniform vec4 camera_position;
};
uniform mat4 u_model_matrix;

void main() {
	gl_Position = projection_matrix * view_matrix * u_model_matrix * a_position;
	mat3 normalMatrix = mat3(transpose(inverse(u_model_matrix)));
	v_uv = a_uv;
	v_normal = a_normals * normalMatrix;
	v_world_pos = (u_model_matrix * a_position).xyz;
}