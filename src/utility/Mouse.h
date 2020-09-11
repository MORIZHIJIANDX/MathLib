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
		
	private:
		FMouse() : mMouseWheelAccumulate(0), mIsInWindow(false) {};
		~FMouse() {};

		void OnMouseButtonPressed(FMouseButtonEventArgs& e);
		void OnMouseButtonReleased(FMouseButtonEventArgs& e);

		void OnMouseMove(FMouseMotionEventArgs& e);
		void OnMouseWheel(FMouseWheelEventArgs& e);
		
		void OnMouseLeave(FMouseMotionEventArgs& e);
		void OnMouseEnter(FMouseMotionEventArgs& e);

		void OnMouseWheelDown();
		void OnMouseWheelUp();

	private:
		bool mIsInWindow;
		FVector2i mMousePos;
		float mMouseWheelAccumulate;
		std::bitset<3> mMouseButtonState;
	};
}