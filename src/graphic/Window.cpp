#include "Window.h"

#include <functional>
#include "../utility/ThreadSafeQueue.h"
#include "../consolid/consolid.h"
#include "../utility/Keyboard.h"
#include "../utility/Mouse.h"

namespace Dash
{
	using MessageFunc = std::function<void()>;
	ThreadSafeQueue<MessageFunc> MessageQueue;

	Window::Window(const std::string& name, const std::string title, size_t width, size_t height)
		: mWindowTitle(title)
		, mWindowWidth(width)
		, mWindowHeight(height)
		, mRequestQuit(false)
	{
		Window::WindowClassRegister::Get(name, ::GetModuleHandle(NULL));
		
		RECT winRect;
		winRect.left = 100;
		winRect.top = 100;
		winRect.right = winRect.left + width;
		winRect.bottom = winRect.top + height;

		::AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);

		//mWindowHandle = CreateWindowEx(
		//	WS_EX_OVERLAPPEDWINDOW,
		//	GetWindowName().c_str(),
		//	title.c_str(),
		//	WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		//	CW_USEDEFAULT, CW_USEDEFAULT,
		//	static_cast<int>(winRect.right - winRect.left), static_cast<int>(winRect.bottom - winRect.top),
		//	nullptr,
		//	nullptr,
		//	Window::WindowClassRegister::Get()->GetWindowInstance(),
		//	this);

		mWindowHandle = CreateWindow(
			GetWindowName().c_str(),
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<int>(winRect.right - winRect.left), 
			static_cast<int>(winRect.bottom - winRect.top),
			nullptr,        // We have no parent window.
			nullptr,        // We aren't using menus.
			Window::WindowClassRegister::Get()->GetWindowInstance(),
			this);

		ShowWindow();
	}

	Window::~Window()
	{
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

		//while (true)
		//{
		//	while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		//	{
		//		if ((msg.message == WM_QUIT) || (mRequestQuit == true))
		//		{
		//			mRequestQuit = false;
		//			return msg.wParam;
		//		}

		//		::TranslateMessage(&msg);
		//		::DispatchMessageA(&msg);
		//	}

		//	std::this_thread::yield();
		//}

		while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_QUIT) || (mRequestQuit == true))
			{
				mRequestQuit = false;
				return msg.wParam;
			}

			::TranslateMessage(&msg);
			::DispatchMessageA(&msg);
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

	// Convert the message ID into a MouseButton ID
	static MouseButton DecodeMouseButton(UINT messageID)
	{
		MouseButton mouseButton = MouseButton::None;
		switch (messageID)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		{
			mouseButton = MouseButton::Left;
			break;
		}
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		{
			mouseButton = MouseButton::Right;
			break;
		}
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		{
			mouseButton = MouseButton::Middle;
			break;
		}
		}

		return mouseButton;
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
			::PostQuitMessage(0);
			return 0;
			// clear keystate when window loses focus to prevent input getting "stuck"
		case WM_KILLFOCUS:
			MessageQueue.Push(std::bind(&Keyboard::ClearStates, &Keyboard::Get()));
			break;

			/*********** KEYBOARD MESSAGES ***********/
		case WM_KEYDOWN:
			// syskey commands need to be handled to track ALT key (VK_MENU) and F10
		case WM_SYSKEYDOWN:
		{	
			MSG charMsg;

			// Get the Unicode character (UTF-16)
			unsigned int c = 0;
			// For printable characters, the next message will be WM_CHAR.
			// This message contains the character code we need to send the KeyPressed event.
			// Inspired by the SDL 1.2 implementation.
			if (PeekMessage(&charMsg, hWnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR)
			{
				GetMessage(&charMsg, hWnd, 0, 0);
				c = static_cast<unsigned int>(charMsg.wParam);
			}

			bool repeat = lParam & 0x40000000;

			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode key = (KeyCode)wParam;
			unsigned int scanCode = (lParam & 0x00FF0000) >> 16;
			KeyEventArgs keyEventArgs(key, c, KeyState::Pressed, control, shift, alt, repeat);
			MessageQueue.Push(std::bind(&Keyboard::OnKeyPressed, &Keyboard::Get(), keyEventArgs));
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode key = (KeyCode)wParam;

			KeyEventArgs keyEventArgs(key, 0, KeyState::Released, control, shift, alt, false);
			MessageQueue.Push(std::bind(&Keyboard::OnKeyReleased, &Keyboard::Get(), keyEventArgs));
			break;
		}
			/*********** END KEYBOARD MESSAGES ***********/

			/************* MOUSE MESSAGES ****************/
		case WM_MOUSEMOVE:
		{
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseMotionEventArgs mouseMotionEventArgs(lButton, mButton, rButton, control, shift, x, y);

			// in client region -> log move, and log enter + capture mouse (if not previously in window)
			if (x >= 0 && x < mWindowWidth && y >= 0 && y < mWindowHeight)
			{
				MessageQueue.Push(std::bind(&Mouse::OnMouseMove, &Mouse::Get(), mouseMotionEventArgs));

				if (!Mouse::Get().IsInWindow())
				{
					SetCapture(hWnd);
					MessageQueue.Push(std::bind(&Mouse::OnMouseEnter, &Mouse::Get(), mouseMotionEventArgs));
				}
			}
			// not in client -> log move / maintain capture if button down
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					MessageQueue.Push(std::bind(&Mouse::OnMouseMove, &Mouse::Get(), mouseMotionEventArgs));
				}
				// button up -> release capture / log event for leaving
				else
				{
					ReleaseCapture();
					MessageQueue.Push(std::bind(&Mouse::OnMouseLeave, &Mouse::Get(), mouseMotionEventArgs));
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(msg), ButtonState::Pressed, lButton, mButton, rButton, control, shift, x, y);
			MessageQueue.Push(std::bind(&Mouse::OnMouseButtonPressed, &Mouse::Get(), mouseButtonEventArgs));
			break;
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			bool lButton = (wParam & MK_LBUTTON) != 0;
			bool rButton = (wParam & MK_RBUTTON) != 0;
			bool mButton = (wParam & MK_MBUTTON) != 0;
			bool shift = (wParam & MK_SHIFT) != 0;
			bool control = (wParam & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			MouseButtonEventArgs mouseButtonEventArgs(DecodeMouseButton(msg), ButtonState::Released, lButton, mButton, rButton, control, shift, x, y);
			MessageQueue.Push(std::bind(&Mouse::OnMouseButtonReleased, &Mouse::Get(), mouseButtonEventArgs));
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			short keyStates = (short)LOWORD(wParam);

			bool lButton = (keyStates & MK_LBUTTON) != 0;
			bool rButton = (keyStates & MK_RBUTTON) != 0;
			bool mButton = (keyStates & MK_MBUTTON) != 0;
			bool shift = (keyStates & MK_SHIFT) != 0;
			bool control = (keyStates & MK_CONTROL) != 0;

			int x = ((int)(short)LOWORD(lParam));
			int y = ((int)(short)HIWORD(lParam));

			// Convert the screen coordinates to client coordinates.
			POINT clientToScreenPoint;
			clientToScreenPoint.x = x;
			clientToScreenPoint.y = y;
			::ScreenToClient(hWnd, &clientToScreenPoint);

			MouseWheelEventArgs mouseWheelEventArgs(zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y);
			MessageQueue.Push(std::bind(&Mouse::OnMouseWheel, &Mouse::Get(), mouseWheelEventArgs));
			break;
		}
		/************** END MOUSE MESSAGES **************/
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
		winClass.style = CS_HREDRAW | CS_VREDRAW;
		winClass.lpfnWndProc = Window::WinProcFunc;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hInstance = mWindowInstance;
		winClass.hIcon = nullptr;
		winClass.hCursor = nullptr;
		//winClass.hbrBackground = nullptr;
		winClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
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