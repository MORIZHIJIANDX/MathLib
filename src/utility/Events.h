#pragma once

#include "KeyCodes.h"
#include "../design_patterns/MulticastDelegate.h"

#include <memory>
#include <string>

namespace Graphics
{
	class Camera;
	class GraphicsCommandBuffer;
}

namespace Dash
{
	class EventArgs
	{
	public:
		EventArgs()
		{}
	};

	using Event = MulticastDelegate<void(EventArgs&)>;
	using EventDelegate = Delegate<void(EventArgs&)>;

	enum class KeyState
	{
		Released = 0,
		Pressed = 1
	};

	class KeyEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;

		KeyEventArgs(KeyCode key, unsigned int c, KeyState state, bool control, bool shift, bool alt, bool repeat)
			: mKey(key)
			, mChar(c)
			, mState(state)
			, mControl(control)
			, mShift(shift)
			, mAlt(alt)
			, mRepeat(repeat)
		{}

		KeyCode         mKey;    // The Key Code that was pressed or released.
		unsigned int    mChar;   // The 32-bit character code that was pressed. This value will be 0 if it is a non-printable character.
		KeyState        mState;  // Was the key pressed or released?
		bool            mControl;// Is the Control modifier pressed
		bool            mShift;  // Is the Shift modifier pressed
		bool            mAlt;    // Is the Alt modifier pressed
		bool			mRepeat;
	};

	using KeyboardEvent = MulticastDelegate<void(KeyEventArgs&)>;
	using KeyboardEventDelegate = Delegate<void(KeyEventArgs&)>;


	class MouseMotionEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;

		MouseMotionEventArgs(bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
			: mLeftButton(leftButton)
			, mMiddleButton(middleButton)
			, mRightButton(rightButton)
			, mControl(control)
			, mShift(shift)
			, mX(x)
			, mY(y)
			, mRelX(0)
			, mRelY(0)
		{}

		bool mLeftButton;    // Is the left mouse button down?
		bool mMiddleButton;  // Is the middle mouse button down?
		bool mRightButton;   // Is the right mouse button down?
		bool mControl;       // Is the CTRL key down?
		bool mShift;         // Is the Shift key down?

		int mX;              // The X-position of the cursor relative to the upper-left corner of the client area.
		int mY;              // The Y-position of the cursor relative to the upper-left corner of the client area.
		int mRelX;			// How far the mouse moved since the last event.
		int mRelY;			// How far the mouse moved since the last event.
	};

	using MouseMotionEvent = MulticastDelegate<void(MouseMotionEventArgs&)>;
	using MouseMotionEventDelegate = Delegate<void(MouseMotionEventArgs&)>;


	enum class MouseButton : unsigned int
	{
		Left = 0x00,
		Right = 0x01,
		Middle = 0x02,
		None = 0x03,
	};

	enum class ButtonState
	{
		Released = 0,
		Pressed = 1
	};

	class MouseButtonEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;

		MouseButtonEventArgs(MouseButton buttonID, ButtonState state, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
			: mButton(buttonID)
			, mState(state)
			, mLeftButton(leftButton)
			, mMiddleButton(middleButton)
			, mRightButton(rightButton)
			, mControl(control)
			, mShift(shift)
			, mX(x)
			, mY(y)
		{}

		MouseButton mButton; // The mouse button that was pressed or released.
		ButtonState mState;  // Was the button pressed or released?
		bool mLeftButton;    // Is the left mouse button down?
		bool mMiddleButton;  // Is the middle mouse button down?
		bool mRightButton;   // Is the right mouse button down?
		bool mControl;       // Is the CTRL key down?
		bool mShift;         // Is the Shift key down?

		int mX;              // The X-position of the cursor relative to the upper-left corner of the client area.
		int mY;              // The Y-position of the cursor relative to the upper-left corner of the client area.
	};

	using MouseButtonEvent = MulticastDelegate<void(MouseButtonEventArgs&)>;
	using MouseButtonEventDelegate = Delegate<void(MouseButtonEventArgs&)>;


	class MouseWheelEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;

		MouseWheelEventArgs(float wheelDelta, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y)
			: mWheelDelta(wheelDelta)
			, mLeftButton(leftButton)
			, mMiddleButton(middleButton)
			, mRightButton(rightButton)
			, mControl(control)
			, mShift(shift)
			, mX(x)
			, mY(y)
		{}

		float mWheelDelta;   // How much the mouse wheel has moved. A positive value indicates that the wheel was moved to the right. A negative value indicates the wheel was moved to the left.
		bool mLeftButton;    // Is the left mouse button down?
		bool mMiddleButton;  // Is the middle mouse button down?
		bool mRightButton;   // Is the right mouse button down?
		bool mControl;       // Is the CTRL key down?
		bool mShift;         // Is the Shift key down?

		int mX;              // The X-position of the cursor relative to the upper-left corner of the client area.
		int mY;              // The Y-position of the cursor relative to the upper-left corner of the client area.

	};

	using MouseWheelEvent = MulticastDelegate<void(MouseWheelEventArgs&)>;
	using MouseWheelEventDelegate = Delegate<void(MouseWheelEventArgs&)>;


	class ResizeEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;

		ResizeEventArgs(int width, int height)
			: mWidth(width)
			, mHeight(height)
		{}

		// The new width of the window
		int mWidth;
		// The new height of the window.
		int mHeight;

	};

	using ResizeEvent = MulticastDelegate<void(ResizeEventArgs&)>;
	using ResizeEventDelegate = Delegate<void(ResizeEventArgs&)>;


	class UpdateEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;
		UpdateEventArgs(double fDeltaTime, double fTotalTime, uint64_t frameCounter)
			: mElapsedTime(fDeltaTime)
			, mTotalTime(fTotalTime)
			, mFrameCounter(frameCounter)
		{}

		double mElapsedTime;
		double mTotalTime;
		uint64_t mFrameCounter;
	};

	using UpdateEvent = MulticastDelegate<void(UpdateEventArgs&)>;
	using UpdateEventDelegate = Delegate<void(UpdateEventArgs&)>;


	class RenderEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;
		RenderEventArgs(double fDeltaTime, double fTotalTime,
			uint64_t frameCounter,
			std::shared_ptr<Graphics::Camera> camera = nullptr,
			std::shared_ptr<Graphics::GraphicsCommandBuffer> graphicsCommandBuffer = nullptr)
			: mElapsedTime(fDeltaTime)
			, mTotalTime(fTotalTime)
			, mFrameCounter(frameCounter)
			, mCamera(camera)
			, mGraphicsCommandBuffer(graphicsCommandBuffer)
		{}

		double mElapsedTime;
		double mTotalTime;
		uint64_t mFrameCounter;

		std::shared_ptr<Graphics::Camera> mCamera;
		std::shared_ptr<Graphics::GraphicsCommandBuffer> mGraphicsCommandBuffer;
	};

	using RenderEvent = MulticastDelegate<void(RenderEventArgs&)>;
	using RenderEventDelegate = Delegate<void(RenderEventArgs&)>;


	class UserEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;
		UserEventArgs(int code, void* data1, void* data2)
			: mCode(code)
			, mData1(data1)
			, mData2(data2)
		{}

		int mCode;
		void* mData1;
		void* mData2;
	};

	using UserEvent = MulticastDelegate<void(UserEventArgs&)>;
	using UserEventDelegate = Delegate<void(UserEventArgs&)>;


	class RuntimeErrorEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;

		RuntimeErrorEventArgs(const std::wstring& errorString, const std::wstring& compilerError)
			: mErrorString(errorString)
			, mCompilerError(compilerError)
		{}

		std::wstring mErrorString;
		std::wstring mCompilerError;
	};

	using RuntimeErrorEvent = MulticastDelegate<void(RuntimeErrorEventArgs&)>;
	using RuntimeErrorEventDelegate = Delegate<void(RuntimeErrorEventArgs&)>;


	class ProgressEventArgs : public EventArgs
	{
	public:
		using base = EventArgs;

		ProgressEventArgs(const std::wstring& fileName, float progress, bool cancel = false)
			: mFileName(fileName)
			, mProgress(progress)
			, mCancel(cancel)
		{}

		// The file that is being loaded.
		std::wstring mFileName;
		// The progress of the loading process.
		float mProgress;
		// Set to TRUE to cancel loading.
		bool mCancel;
	};

	using ProgressEvent = MulticastDelegate<void(ProgressEventArgs&)>;
	using ProgressEventDelegate = Delegate<void(ProgressEventArgs&)>;


	enum class FileAction
	{
		Unknown,        // An unknown action triggered this event. (Should not happen, but I guess its possible)
		Added,          // A file was added to a directory.
		Removed,        // A file was removed from a directory.
		Modified,       // A file was modified. This might indicate the file's timestamp was modified or another attribute was modified.
		RenameOld,      // The file was renamed and this event stores the previous name.
		RenameNew,      // The file was renamed and this event stores the new name.
	};

	class FileChangeEventArgs : EventArgs
	{
	public:
		using base = EventArgs;

		FileChangeEventArgs(FileAction action, const std::wstring& path)
			: mAction(action)
			, mPath(path)
		{}

		FileAction mAction; // The action that triggered this event.
						   // The file or directory path that was modified.
		std::wstring mPath;
	};

	using FileChangeEvent = MulticastDelegate<void(FileChangeEventArgs&)>;
	using FileChangeEventDelegate = Delegate<void(FileChangeEventArgs&)>;
}
