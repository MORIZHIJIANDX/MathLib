#pragma once
#include "Window.h"
#include "../utility/Events.h"
#include <atomic>

namespace Dash
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual int Run();
		virtual void Stop();

		virtual void OnRender(const RenderEventArgs& e);
		virtual void OnUpdate(const UpdateEventArgs& e);

	private:
		void UpdateThread();

		Window mWindow;
		std::atomic_bool mIsRunning;
	};
}
