#include "Window.h"

#include <functional>
#include "../utility/ThreadSafeQueue.h"
#include "../consolid/consolid.h"

namespace Dash
{
	using MessageFunc = std::function<void()>;
	ThreadSafeQueue<MessageFunc> MessageQueue;

	Window::Window(HINSTANCE inst, const std::string& name, const std::string title, size_t width, size_t height)
		: mWindowTitle(title)
		, mWindowWidth(width)
		, mWindowHeight(height)
		, mRequestQuit(false)
	{
		Window::WindowClassRegister::Get(name, inst);
		
		mWindowHandle = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW, 
			GetWindowName().c_str(),
			title.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			0, 0,
			static_cast<int>(width), static_cast<int>(height),
			nullptr,
			nullptr,
			inst,
			this);

		ShowWindow();
	}

	Window::~Window()
	{
		mRequestQuit = true;
		::DestroyWindow(mWindowHandle);
	}

	void Window::ShowWindow() noexcept
	{
		ASSERT(mWindowHandle != nullptr);

		::ShowWindow(mWindowHandle, SW_SHOWDEFAULT);
	}

	void Window::HideWindow() noexcept
	{
		ASSERT(mWindowHandle != nullptr);

		::ShowWindow(mWindowHandle, SW_HIDE);
	}

	void Window::CloseWindow() noexcept
	{
		mRequestQuit = true;
	}

	void Window::SetTitle(const std::string& title) noexcept
	{
		ASSERT(mWindowHandle != nullptr);

		::SetWindowText(mWindowHandle, title.c_str());
	}

	void Window::SetFullScreen(bool fullScreen) noexcept
	{
		if (mFullScreen != fullScreen)
		{
			mFullScreen = fullScreen;
		}
	}

	bool Window::IsFullScreen() const noexcept
	{
		return mFullScreen;
	}

	int Window::WindowsMessageLoop() noexcept
	{
		MSG msg = {};

		while (true)
		{
			while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessageA(&msg);

				if (msg.message == WM_QUIT || mRequestQuit == true)
				{
					mRequestQuit = false;
					return msg.wParam;
				}
			}

			std::this_thread::yield();
		}
	}

	void Window::ProcessMessage() noexcept
	{
		MessageFunc func;
		while (MessageQueue.TryPop(func))
		{
			func();
		}
	}

	LRESULT Window::WinProcFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window instance
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
			// forward message to window instance handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// retrieve ptr to window instance
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
			// we don't want the DefProc to handle this message because
			// we want our destructor to destroy the window, so return 0 instead of break
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			// clear keystate when window loses focus to prevent input getting "stuck"
		case WM_KILLFOCUS:
			kbd.ClearState();
			break;
		case WM_ACTIVATE:
			// confine/free cursor on window to foreground/background if cursor disabled
			if (!cursorEnabled)
			{
				if (wParam & WA_ACTIVE)
				{
					ConfineCursor();
					HideCursor();
				}
				else
				{
					FreeCursor();
					ShowCursor();
				}
			}
			break;

			/*********** KEYBOARD MESSAGES ***********/
		case WM_KEYDOWN:
			// syskey commands need to be handled to track ALT key (VK_MENU) and F10
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
			{
				kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			kbd.OnChar(static_cast<unsigned char>(wParam));
			break;
			/*********** END KEYBOARD MESSAGES ***********/

			/************* MOUSE MESSAGES ****************/
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			// cursorless exclusive gets first dibs
			if (!cursorEnabled)
			{
				if (!mouse.IsInWindow())
				{
					SetCapture(hWnd);
					mouse.OnMouseEnter();
					HideCursor();
				}
				break;
			}
			// in client region -> log move, and log enter + capture mouse (if not previously in window)
			if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
			{
				mouse.OnMouseMove(pt.x, pt.y);
				if (!mouse.IsInWindow())
				{
					SetCapture(hWnd);
					mouse.OnMouseEnter();
				}
			}
			// not in client -> log move / maintain capture if button down
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					mouse.OnMouseMove(pt.x, pt.y);
				}
				// button up -> release capture / log event for leaving
				else
				{
					ReleaseCapture();
					mouse.OnMouseLeave();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SetForegroundWindow(hWnd);
			if (!cursorEnabled)
			{
				ConfineCursor();
				HideCursor();
			}
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.OnWheelDelta(pt.x, pt.y, delta);
			break;
		}
		/************** END MOUSE MESSAGES **************/

		/************** RAW MOUSE MESSAGES **************/
		case WM_INPUT:
		{
			if (!mouse.RawEnabled())
			{
				break;
			}
			UINT size;
			// first get the size of the input data
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				nullptr,
				&size,
				sizeof(RAWINPUTHEADER)) == -1)
			{
				// bail msg processing if error
				break;
			}
			rawBuffer.resize(size);
			// read in the input data
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				rawBuffer.data(),
				&size,
				sizeof(RAWINPUTHEADER)) != size)
			{
				// bail msg processing if error
				break;
			}
			// process the raw input data
			auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
			if (ri.header.dwType == RIM_TYPEMOUSE &&
				(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
			{
				mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
			}
			break;
		}
		/************** END RAW MOUSE MESSAGES **************/
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	HINSTANCE Window::WindowClassRegister::GetWindowInstance() const noexcept
	{
		return mWindowInstance;
	}

	const std::string& Window::WindowClassRegister::GetWindowClassName() const noexcept
	{
		return mWindowClassName;
	}

	Window::WindowClassRegister::WindowClassRegister()
		: mWindowClassName()
		, mWindowInstance(nullptr)
	{
	}

	Window::WindowClassRegister::WindowClassRegister(const std::string& windowName, HINSTANCE windowInstance)
		: mWindowClassName(windowName)
		, mWindowInstance(windowInstance)
	{
		WNDCLASSEX winClass;
		ZeroMemory(&winClass, sizeof(WNDCLASSEX));
		
		winClass.cbSize = sizeof(winClass);
		winClass.style = CS_OWNDC;
		winClass.lpfnWndProc = Window::WinProcFunc;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = mWindowInstance;
		winClass.hIcon = nullptr;
		winClass.hCursor = nullptr;
		winClass.hbrBackground = nullptr;
		winClass.lpszMenuName = nullptr;
		winClass.lpszClassName = mWindowClassName.c_str();
		winClass.hIconSm = nullptr;

		RegisterClassEx(&winClass);
	}

	Window::WindowClassRegister::~WindowClassRegister()
	{
		UnregisterClass(mWindowClassName.c_str(), mWindowInstance);
	}
}