#include "Keyboard.h"

namespace Dash
{
	FKeyboard& FKeyboard::Get()
	{
		static FKeyboard instance;
		return instance;
	}

	bool FKeyboard::IsKeyPressed(EKeyCode key) const
	{
		return mKeyStates[static_cast<unsigned int>(key)];
	}

	std::optional<char> FKeyboard::ReadChar()
	{
		if (mCharBuffer.size() > 0u)
		{
			unsigned char character = mCharBuffer.front();
			mCharBuffer.pop();
			return character;
		}
		return {};
	}

	void FKeyboard::OnKeyPressed(FKeyEventArgs& e)
	{
		if (!e.mRepeat || IsAutoRepeatEnabled())
		{
			mKeyStates[static_cast<unsigned int>(e.mKey)] = true;
		}
		
		if (e.mChar != 0)
		{
			OnChar(e.mChar);
		}

		KeyPressed(e);
	}

	void FKeyboard::OnKeyReleased(FKeyEventArgs& e)
	{
		mKeyStates[static_cast<unsigned int>(e.mKey)] = false;

		KeyReleased(e);
	}


	void FKeyboard::OnChar(char character)
	{
		mCharBuffer.push(character);
		TrimBuffer(mCharBuffer);
	}

	template<typename T>
	inline void FKeyboard::TrimBuffer(std::queue<T>& buffer)
	{
		static const size_t bufferSize = 16;
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}

	void FKeyboard::ClearStates()
	{
		mKeyStates.reset();
	}

	void FKeyboard::FlushCharBuffer()
	{
		mCharBuffer = std::queue<char>();
	}

	void FKeyboard::EnableAutoRepeat()
	{
		mAutoRepeat = true;
	}

	void FKeyboard::DisableAutoRepeat()
	{
		mAutoRepeat = false;
	}

	bool FKeyboard::IsAutoRepeatEnabled() const
	{
		return mAutoRepeat;
	}

}