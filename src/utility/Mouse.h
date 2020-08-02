#pragma once

#include <bitset>
#include "Events.h"
#include "../math/MathType.h"

namespace Dash
{
	class Mouse
	{
		friend class Window;
	public:
		static Mouse& Get();
		
		bool IsMouseButtonPressed(MouseButton button) const;
		bool IsInWindow() const;
		Vector2i GetMousePos() const;

	private:
		Mouse() : mMouseWheelAccumulate(0), mIsInWindow(false) {};
		~Mouse() {};

		void OnMouseButtonPressed(const MouseButtonEventArgs& e);
		void OnMouseButtonReleased(const MouseButtonEventArgs& e);

		void OnMouseMove(const MouseMotionEventArgs& e);
		void OnMouseWheel(const MouseWheelEventArgs& e);
		
		void OnMouseLeave(const MouseMotionEventArgs& e);
		void OnMouseEnter(const MouseMotionEventArgs& e);

		void OnMouseWheelDown();
		void OnMouseWheelUp();

	private:
		bool mIsInWindow;
		Vector2i mMousePos;
		float mMouseWheelAccumulate;
		std::bitset<3> mMouseButtonState;
	};
}