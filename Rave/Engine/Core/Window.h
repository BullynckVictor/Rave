#pragma once
#include "Engine/Core/SystemInclude.h"
#include "Engine/Utility/Result.h"
#include "Engine/Utility/types.h"
#include "Engine/Utility/Vector.h"

namespace rv
{
	namespace win32
	{
		class WindowClass
		{
		public:
			WindowClass() = default;

			static Result Create(WindowClass& wnd, const char* name, WNDPROC proc);			
			const WNDCLASSEX& Info() const;
			const ATOM& Identifier() const;

		private:
			WNDCLASSEX wc { sizeof(WNDCLASSEX) };
			ATOM atom{};
		};

		struct WindowCreateInfo
		{
			std::string title;
			Extent<uint> size = 0;
		};

		class Window
		{
		public:
			~Window();

			static Result Create(Window& window, const std::string& title, uint width, uint height);
			static Result Create(Window& window, const WindowCreateInfo& info);

			const Extent<uint>& Size() const;

			const std::string& Title() const;
			Result SetTitle(const std::string& title);
			Result SetTitle(std::string&& title);

			bool Open();
			Result Close();

			bool HandleMessages();

			HWND Handle() const;

		private:
			static Result CreateWindowClass();

			static LRESULT CALLBACK WindowSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
			static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
			LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			static WindowClass wClass;
			WindowCreateInfo data;
			HWND hwnd;
		};
	}

	typedef win32::WindowCreateInfo WindowCreateInfo;
	typedef win32::Window Window;
}