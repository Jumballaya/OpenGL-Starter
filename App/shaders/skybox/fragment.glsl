#version 460 core

layout (location=0) in vec3 v_dir;
layout (location=0) out vec4 outColor;
layout (binding=1) uniform samplerCube texture1;

void main()
{
	vec4 color = texture(texture1, v_dir);
	outColor = color;
};
