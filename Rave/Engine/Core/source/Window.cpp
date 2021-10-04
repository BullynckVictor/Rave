#include "Engine/Core/Window.h"
#include "Engine/Utility/Error.h"

namespace rv
{
#	ifdef RV_PLATFORM_WINDOWS

	namespace win32
	{
		WindowClass Window::wClass{};

		Result WindowClass::Create(WindowClass& wnd, const char* name, WNDPROC proc)
		{
			rv_result;

			wnd.wc.lpszClassName = name;
			wnd.wc.hInstance = GetModuleHandle(nullptr);
			wnd.wc.lpfnWndProc = proc;

			wnd.atom = RegisterClassEx(&wnd.wc);
			rvrlrof(wnd.atom);

			return result;
		}

		const WNDCLASSEX& WindowClass::Info() const
		{
			return wc;
		}

		const ATOM& WindowClass::Identifier() const
		{
			return atom;
		}

		Window::~Window()
		{
			Close();
		}

		Result Window::Create(Window& window, const std::string& title, uint width, uint height)
		{
			WindowInfo info{};
			info.title = title;
			info.size = { width, height };
			return Window::Create(window, info);
		}

		Result Window::Create(Window& window, const WindowInfo& info)
		{
			rv_result;

			if (!wClass.Identifier())
				rvrf(CreateWindowClass());

			window.data = info;
			
			DWORD style = WS_OVERLAPPEDWINDOW;

			window.hwnd = CreateWindowEx(
				0,
				wClass.Info().lpszClassName,
				window.data.title.c_str(),
				style,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				window.data.size.width,
				window.data.size.height,
				nullptr,
				nullptr,
				wClass.Info().hInstance,
				&window
			);
			rvrlrof(window.hwnd);

			ShowWindow(window.hwnd, SW_NORMAL);

			return result;
		}

		Extent<uint> Window::Size() const
		{
			return data.size;
		}

		const std::string& Window::Title() const
		{
			return data.title;
		}

		void Window::SetTitle(const std::string& title)
		{
			data.title = title;
			SetWindowText(hwnd, title.c_str());
		}

		void Window::SetTitle(std::string&& title)
		{
			data.title = std::move(title);
			SetWindowText(hwnd, title.c_str());
		}

		bool Window::Open()
		{
			return hwnd;
		}

		Result Window::Close()
		{
			return rv_last_result_onfail(DestroyWindow(hwnd));
		}

		bool Window::HandleMessages()
		{
			MSG msg;
			while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return Open();
		}

		Result Window::CreateWindowClass()
		{
			return WindowClass::Create(wClass, "RaveEngine win32 Window Class", WindowSetup);
		}
		
		LRESULT Window::WindowSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (msg != WM_CREATE)
				return DefWindowProc(hwnd, msg, wParam, lParam);

			LPCREATESTRUCT create = reinterpret_cast<LPCREATESTRUCT>(lParam);
			if (create)
			{
				if (create->lpCreateParams)
				SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::StaticWndProc));
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		LRESULT Window::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (wnd)
				return wnd->WndProc(hwnd, msg, wParam, lParam);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
				case WM_CLOSE:
				{
					Close();
					return 0;
				}
				case WM_DESTROY:
				{
					this->hwnd = nullptr;
					return 0;
				}
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

#	endif
}