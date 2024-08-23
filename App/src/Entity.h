#pragma once

#include <string>

#include <Core/Core.h>

class Entity {
public:
	Core::Render::Transform transform;

	Entity(Core::Render::Model* m, Core::GL::Shader* s) : transform(Core::Render::Transform()), shader(s), model(m) {
	};
	~Entity() = default;

	void draw();

private:
	Core::GL::Shader* shader;
	Core::Render::Model* model;
};