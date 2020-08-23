#include "Mouse.h"

#include "../utility/LogManager.h"
#include "../utility/LogStream.h"

namespace Dash
{
	Mouse& Mouse::Get()
	{	
		static Mouse instance;
		return instance;
	}

	bool Mouse::IsMouseButtonPressed(MouseButton button) const
	{
		return mMouseButtonState[static_cast<unsigned int>(button)];
	}

	bool Mouse::IsInWindow() const
	{
		return mIsInWindow;
	}

	Vector2i Mouse::GetMousePos() const
	{
		return mMousePos;
	}

	void Mouse::OnMouseButtonPressed(MouseButtonEventArgs& e)
	{
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Left)] = e.mLeftButton;
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Middle)] = e.mMiddleButton;
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Right)] = e.mRightButton;

		mMousePos.x = e.mX;
		mMousePos.y = e.mY;

		MouseButtonPressed(e);

		LOG_INFO << "Button : " << static_cast<unsigned int>(e.mButton) << " , State : " << static_cast<unsigned int>(e.mState);
	}

	void Mouse::OnMouseButtonReleased(MouseButtonEventArgs& e)
	{
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Left)] = e.mLeftButton;
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Middle)] = e.mMiddleButton;
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Right)] = e.mRightButton;

		mMousePos.x = e.mX;
		mMousePos.y = e.mY;

		MouseButtonReleased(e);
	}

	void Mouse::OnMouseMove(MouseMotionEventArgs& e)
	{
		mMousePos.x = e.mX;
		mMousePos.y = e.mY;

		MouseMoved(e);
	}

	void Mouse::OnMouseWheel(MouseWheelEventArgs& e)
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

	void Mouse::OnMouseLeave(MouseMotionEventArgs& e)
	{
		mIsInWindow = false;
	}

	void Mouse::OnMouseEnter(MouseMotionEventArgs& e)
	{
		mIsInWindow = true;
	}

	void Mouse::OnMouseWheelDown()
	{
		LOG_INFO << "Mouse Wheel Down";
	}

	void Mouse::OnMouseWheelUp()
	{
		LOG_INFO << "Mouse Wheel Up";
	}
}