#pragma once

#include <bitset>
#include "Events.h"
#include "../math/MathType.h"

namespace Dash
{
	class FMouse
	{
		friend class FWindow;
	public:
		static FMouse& Get();
		
		bool IsMouseButtonPressed(EMouseButton button) const;
		bool IsInWindow() const;
		FVector2i GetMousePos() const;
	
	public:

		FMouseButtonEvent MouseButtonPressed;
		FMouseButtonEvent MouseButtonReleased;
		FMouseMotionEvent MouseMoved;
		FMouseWheelEvent MouseWheelUp;
		FMouseWheelEvent MouseWheelDown;
		
	private:
		FMouse() : mMouseWheelAccumulate(0), mIsInWindow(false) {};
		~FMouse() {};

		void OnMouseButtonPressed(FMouseButtonEventArgs& e);
		void OnMouseButtonReleased(FMouseButtonEventArgs& e);

		void OnMouseMove(FMouseMotionEventArgs& e);
		void OnMouseWheel(FMouseWheelEventArgs& e);
		
		void OnMouseLeave(FMouseMotionEventArgs& e);
		void OnMouseEnter(FMouseMotionEventArgs& e);

		void OnMouseWheelDown(FMouseWheelEventArgs& e);
		void OnMouseWheelUp(FMouseWheelEventArgs& e);

	private:
		bool mIsInWindow;
		FVector2i mMousePos;
		float mMouseWheelAccumulate;
		std::bitset<3> mMouseButtonState;
	};
}