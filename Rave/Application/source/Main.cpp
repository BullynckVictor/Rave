#include <iostream>
#include <functional>
#include <thread>
#include <thread>
#include "Engine/Engine.h"

using namespace std::chrono_literals;

rv::Result rave_main()
{
	rv::Window window{};
	rv::Window::Create(window, "Hello world!", 600, 400).expect("Unable to create window");

	while (window.HandleMessages())
	{
	}

	return rv::success;
}