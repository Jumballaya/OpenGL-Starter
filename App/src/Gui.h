#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Core/GL/gl.h"

class Gui {
public:
	Gui();
	~Gui();

	void Draw(int width, int height);

	void setupCallbacks(GLFWwindow* window);

private:
	GLuint vao;
	GLuint bufferVertex;
	GLuint bufferElements;
	GLuint perFrameDataBuffer;
	GLuint textureFont;
	Core::GL::Shader guiShader;
};