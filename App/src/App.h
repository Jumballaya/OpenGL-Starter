#pragma once
#include <functional>
#include <Core/Core.h>
#include "Window.h"
#include "app_data.h"
#include "Gui.h"
#include "Entity.h"

class App {
public:
	Window window;
	Gui gui;
	Core::Render::Camera camera;
	Core::Render::Skybox skybox;

	App();
	~App();

	void setup();
	void destroy();

	void run(const std::function <void (App*, double, int, int)>& fn);

	Core::IO::Inputs inputs;
};