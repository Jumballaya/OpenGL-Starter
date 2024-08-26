#pragma once
#include <functional>

#include <Core/Core.h>
#include "Window.h"
#include "Gui.h"
#include "Entity.h"



class App {
public:
	Window window;
	// Gui gui;
	Core::Render::Camera camera;

	App();
	~App();

	void setup();
	void destroy();

	void run(const std::function <void (App*, int, int)>& fn);
};