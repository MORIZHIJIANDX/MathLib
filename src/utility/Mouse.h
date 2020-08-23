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
	
	public:

		MouseButtonEvent MouseButtonPressed;
		MouseButtonEvent MouseButtonReleased;
		MouseMotionEvent MouseMoved;
		
	private:
		Mouse() : mMouseWheelAccumulate(0), mIsInWindow(false) {};
		~Mouse() {};

		void OnMouseButtonPressed(MouseButtonEventArgs& e);
		void OnMouseButtonReleased(MouseButtonEventArgs& e);

		void OnMouseMove(MouseMotionEventArgs& e);
		void OnMouseWheel(MouseWheelEventArgs& e);
		
		void OnMouseLeave(MouseMotionEventArgs& e);
		void OnMouseEnter(MouseMotionEventArgs& e);

		void OnMouseWheelDown();
		void OnMouseWheelUp();

	private:
		bool mIsInWindow;
		Vector2i mMousePos;
		float mMouseWheelAccumulate;
		std::bitset<3> mMouseButtonState;
	};
}