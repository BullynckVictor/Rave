#include <iostream>
#include <functional>
#include <thread>
#include <thread>
#include "Engine/Engine.h"

using namespace std::chrono_literals;

struct A
{

};

struct B : public A
{

};

rv::Result rave_main()
{	
	rv::GraphicsEngine engine{};
	rv::WindowRenderer* renderer;

	rv::GraphicsEngine::Create(engine).expect("Unable to create engine");
	renderer = engine.CreateRenderer("main window", "Hello world from Direct3D11!", 600, 400, true).get("Unable to create window renderer");

	while (renderer->window.HandleMessages())
	{
		engine.Render().expect("Unable to render");
		renderer->Present().expect("Unable to present");
	}

	return rv::success;
}