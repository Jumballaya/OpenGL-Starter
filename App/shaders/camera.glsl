layout(std140, binding = 0) uniform camera {
	uniform mat4 view_matrix;
	uniform mat4 projection_matrix;
};