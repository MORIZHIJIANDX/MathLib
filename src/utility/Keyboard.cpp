#include "Keyboard.h"

namespace Dash
{
	Keyboard& Keyboard::Get()
	{
		static Keyboard instance;
		return instance;
	}

	bool Keyboard::IsKeyPressed(KeyCode key) const
	{
		return mKeyStates[static_cast<unsigned int>(key)];
	}

	std::optional<char> Keyboard::ReadChar()
	{
		if (mCharBuffer.size() > 0u)
		{
			unsigned char character = mCharBuffer.front();
			mCharBuffer.pop();
			return character;
		}
		return {};
	}

	void Keyboard::OnKeyPressed(KeyEventArgs& e)
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

	void Keyboard::OnKeyReleased(KeyEventArgs& e)
	{
		mKeyStates[static_cast<unsigned int>(e.mKey)] = false;

		KeyReleased(e);
	}


	void Keyboard::OnChar(char character)
	{
		mCharBuffer.push(character);
		TrimBuffer(mCharBuffer);
	}

	template<typename T>
	inline void Keyboard::TrimBuffer(std::queue<T>& buffer)
	{
		static const size_t bufferSize = 16;
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}

	void Keyboard::ClearStates()
	{
		mKeyStates.reset();
	}

	void Keyboard::FlushCharBuffer()
	{
		mCharBuffer = std::queue<char>();
	}

	void Keyboard::EnableAutoRepeat()
	{
		mAutoRepeat = true;
	}

	void Keyboard::DisableAutoRepeat()
	{
		mAutoRepeat = false;
	}

	bool Keyboard::IsAutoRepeatEnabled() const
	{
		return mAutoRepeat;
	}

}