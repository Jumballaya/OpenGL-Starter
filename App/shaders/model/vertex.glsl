#version 460 core

layout(location=0) in vec4 a_position;
layout(location=1) in vec3 a_normals;
layout(location=2) in vec2 a_uv;

out vec2 v_uv;

layout(std140, binding = 0) uniform camera {
	uniform mat4 view_matrix;
	uniform mat4 projection_matrix;
};

uniform mat4 u_model_matrix;


void main() {
	gl_Position = projection_matrix * view_matrix * u_model_matrix * a_position;
	v_uv = a_uv;
}