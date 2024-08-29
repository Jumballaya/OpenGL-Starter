#pragma once

#include <string>

#include <Core/Core.h>

class Entity {
public:
	Core::Render::Transform transform;

	Entity() = default;
	~Entity() = default;

	void setup();
	void loadShader(std::string vertex, std::string fragment);
	void loadModel(std::string path);
	void destroy();

	void draw();

	Core::Render::Model model;
	Core::GL::Shader shader;
private:
};