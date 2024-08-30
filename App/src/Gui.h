#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include <Core/GL/gl.h>

struct GuiData {
public:
	glm::mat4 data;
};

class Gui {
public:
	Gui();
	~Gui();

	void setup();
	void destroy();

	void startFrame(int width, int height);
	void draw(int width, int height);

	void setCursorPosCallback(GLFWwindow* window, double x, double y);
	void setMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void setScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	void setCharCallback(GLFWwindow* window, unsigned int c);
	void setKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Components
	void drawTextureWindowGL(const char* title, uint32_t texId);
	void drawFPSDisplayGL(const float fps);
	void drawDemo();

private:
	GuiData perFrameData;
	Core::GL::VertexArray vao;
	Core::GL::UniformBuffer<GuiData> perFrameUbo;
	Core::GL::Shader guiShader;
	Core::GL::Texture texFont;
};