#include "Mouse.h"

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

	void Mouse::OnMouseButtonPressed(const MouseButtonEventArgs& e)
	{
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Left)] = e.mLeftButton;
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Middle)] = e.mMiddleButton;
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Right)] = e.mRightButton;

		mMousePos.x = e.mX;
		mMousePos.y = e.mY;
	}

	void Mouse::OnMouseButtonReleased(const MouseButtonEventArgs& e)
	{
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Left)] = e.mLeftButton;
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Middle)] = e.mMiddleButton;
		mMouseButtonState[static_cast<unsigned int>(MouseButton::Right)] = e.mRightButton;

		mMousePos.x = e.mX;
		mMousePos.y = e.mY;
	}

	void Mouse::OnMouseMove(const MouseMotionEventArgs& e)
	{
		mMousePos.x = e.mX;
		mMousePos.y = e.mY;
	}

	void Mouse::OnMouseWheel(const MouseWheelEventArgs& e)
	{
		mMouseWheelAccumulate += e.m_WheelDelta;

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

	void Mouse::OnMouseLeave(const MouseMotionEventArgs& e)
	{
		mIsInWindow = false;
	}

	void Mouse::OnMouseEnter(const MouseMotionEventArgs& e)
	{
		mIsInWindow = true;
	}

	void Mouse::OnMouseWheelDown()
	{
	}

	void Mouse::OnMouseWheelUp()
	{
	}
}