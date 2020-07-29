#pragma once

#include <string>
#include <memory>
#include <thread>
#include <Windows.h>
#include "../design_patterns/Singleton.h"

namespace Dash
{
	class Window
	{
	public:
		//friend class WindowClassRegister;

		Window(HINSTANCE inst, const std::string& name, const std::string title, size_t width = 1080, size_t height = 720);
		virtual ~Window();

		const std::string& GetWindowName() const noexcept { return WindowClassRegister::Get()->GetWindowClassName(); }

		size_t GetWindowWidth() const noexcept { return mWindowWidth; }
		size_t GetWindowHeight() const noexcept { return mWindowHeight; }

		virtual void ShowWindow() noexcept;
		virtual void HideWindow() noexcept;
		virtual void CloseWindow() noexcept;

		void SetTitle(const std::string& title) noexcept;
		virtual void SetFullScreen(bool fullScreen) noexcept;
		
		bool IsFullScreen() const noexcept;

		int WindowsMessageLoop() noexcept;

		void ProcessMessage() noexcept;

	private:

		static LRESULT CALLBACK WinProcFunc(HWND, UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK HandleMsgThunk(HWND, UINT, WPARAM, LPARAM);
		LRESULT CALLBACK HandleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;

		class WindowClassRegister : public Singleton<WindowClassRegister>
		{
		public:
			HINSTANCE GetWindowInstance() const noexcept;
			const std::string& GetWindowClassName() const noexcept;

			WindowClassRegister();
			WindowClassRegister(const std::string& windowName, HINSTANCE windowInstance);
			~WindowClassRegister();
			
			WindowClassRegister(WindowClassRegister&) = delete;
			WindowClassRegister& operator=(const WindowClassRegister&) = delete;

		private:

			std::string mWindowClassName;
			HINSTANCE mWindowInstance;
		};

		HWND mWindowHandle;

		size_t mWindowWidth;
		size_t mWindowHeight;

		std::string mWindowTitle;

		bool mFullScreen;
		bool mRequestQuit;
	};
}
