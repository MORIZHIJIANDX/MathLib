#include "Application.h"
#include "../consolid/consolid.h"
#include "../utility/HighResolutionTimer.h"
#include "../utility/Keyboard.h"

namespace Dash
{
	#define RENDER_WINDOW_CLASS_NAME "RenderWindowClass"

	Application::Application()
		: mWindow(RENDER_WINDOW_CLASS_NAME, "Test")
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

		std::thread updateThread = std::thread{&Application::UpdateThread, this};
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
}
