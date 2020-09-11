#include "Mouse.h"

#include "../utility/LogManager.h"
#include "../utility/LogStream.h"

namespace Dash
{
	FMouse& FMouse::Get()
	{	
		static FMouse instance;
		return instance;
	}

	bool FMouse::IsMouseButtonPressed(EMouseButton button) const
	{
		return mMouseButtonState[static_cast<unsigned int>(button)];
	}

	bool FMouse::IsInWindow() const
	{
		return mIsInWindow;
	}

	FVector2i FMouse::GetMousePos() const
	{
		return mMousePos;
	}

	void FMouse::OnMouseButtonPressed(FMouseButtonEventArgs& e)
	{
		mMouseButtonState[static_cast<unsigned int>(EMouseButton::Left)] = e.mLeftButton;
		mMouseButtonState[static_cast<unsigned int>(EMouseButton::Middle)] = e.mMiddleButton;
		mMouseButtonState[static_cast<unsigned int>(EMouseButton::Right)] = e.mRightButton;

		mMousePos.x = e.mX;
		mMousePos.y = e.mY;

		MouseButtonPressed(e);

		LOG_INFO << "Button : " << static_cast<unsigned int>(e.mButton) << " , State : " << static_cast<unsigned int>(e.mState);
	}

	void FMouse::OnMouseButtonReleased(FMouseButtonEventArgs& e)
	{
		mMouseButtonState[static_cast<unsigned int>(EMouseButton::Left)] = e.mLeftButton;
		mMouseButtonState[static_cast<unsigned int>(EMouseButton::Middle)] = e.mMiddleButton;
		mMouseButtonState[static_cast<unsigned int>(EMouseButton::Right)] = e.mRightButton;

		mMousePos.x = e.mX;
		mMousePos.y = e.mY;

		MouseButtonReleased(e);
	}

	void FMouse::OnMouseMove(FMouseMotionEventArgs& e)
	{
		mMousePos.x = e.mX;
		mMousePos.y = e.mY;

		MouseMoved(e);
	}

	void FMouse::OnMouseWheel(FMouseWheelEventArgs& e)
	{
		mMouseWheelAccumulate += e.mWheelDelta;

		if (mMouseWheelAccumulate >= WHEEL_DELTA)
		{
			mMouseWheelAccumulate -= WHEEL_DELTA;
			OnMouseWheelUp();
		}
		else if (mMouseWheelAccumulate <= -WHEEL_DELTA)
		{
			mMouseWheelAccumulate += WHEEL_DELTA;
			OnMouseWheelDown();
		}
	}

	void FMouse::OnMouseLeave(FMouseMotionEventArgs& e)
	{
		mIsInWindow = false;
	}

	void FMouse::OnMouseEnter(FMouseMotionEventArgs& e)
	{
		mIsInWindow = true;
	}

	void FMouse::OnMouseWheelDown()
	{
		LOG_INFO << "FMouse Wheel Down";
	}

	void FMouse::OnMouseWheelUp()
	{
		LOG_INFO << "FMouse Wheel Up";
	}
}