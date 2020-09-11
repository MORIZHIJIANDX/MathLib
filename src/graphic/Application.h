#pragma once
#include "Window.h"
#include "../utility/Events.h"
#include <atomic>

namespace Dash
{
	class FApplication
	{
	public:
		FApplication(size_t windowWidth, size_t windowHeight);
		virtual ~FApplication();

		virtual int Run();
		virtual void Stop();

		virtual void OnRender(const FRenderEventArgs& e);
		virtual void OnUpdate(const FUpdateEventArgs& e);

	protected:
		void UpdateThread();

		FWindow mWindow;
		std::atomic_bool mIsRunning;
	};
}

