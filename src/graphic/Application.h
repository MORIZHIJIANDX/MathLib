#pragma once
#include "Window.h"
#include "../utility/Events.h"
#include <atomic>

namespace Dash
{
	class Application
	{
	public:
		Application(size_t windowWidth, size_t windowHeight);
		virtual ~Application();

		virtual int Run();
		virtual void Stop();

		virtual void OnRender(const RenderEventArgs& e);
		virtual void OnUpdate(const UpdateEventArgs& e);

	protected:
		void UpdateThread();

		Window mWindow;
		std::atomic_bool mIsRunning;
	};
}

