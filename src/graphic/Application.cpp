#include "Application.h"
#include "../consolid/consolid.h"
#include "../utility/HighResolutionTimer.h"
#include "../utility/Keyboard.h"

#include "../utility/Exception.h"

namespace Dash
{
	#define RENDER_WINDOW_CLASS_NAME "RenderWindowClass"

	Application::Application(size_t windowWidth, size_t windowHeight )
		: mWindow(RENDER_WINDOW_CLASS_NAME, "Test", windowWidth, windowHeight)
		, mIsRunning(false)
	{
		
	}

	Application::~Application()
	{
	}

	int Application::Run()
	{
		ASSERT(!mIsRunning);

		mIsRunning = true;

		std::thread updateThread = std::thread{ &Application::UpdateThread, this };
		SetThreadName(updateThread, "UpdateThread");

		int returnCode = mWindow.WindowsMessageLoop();

		mIsRunning = false;

		if (updateThread.joinable())
		{
			updateThread.join();
		}

		return returnCode;
	}

	void Application::Stop()
	{
		mWindow.CloseWindow();
	}

	void Application::OnRender(const RenderEventArgs& e)
	{
	}

	void Application::OnUpdate(const UpdateEventArgs& e)
	{
	}

	void Application::UpdateThread()
	{
		try
		{
			HighResolutionTimer timer;

			size_t frameCount = 0;

			while (mIsRunning)
			{
				timer.Update();

				double deltaTime = timer.DeltaSeconds();
				double elapsedTime = timer.ElapsedSeconds();

				mWindow.ProcessMessage();

				OnUpdate(UpdateEventArgs{ deltaTime, elapsedTime, frameCount });
				OnRender(RenderEventArgs{ deltaTime, elapsedTime, frameCount });

				++frameCount;

				std::this_thread::yield();
			}
		}
		catch (const HrException& e)
		{
			MessageBoxA(NULL, HrToString(e.Error()).c_str(), "Error", MB_ICONERROR);
		}
	}
}
