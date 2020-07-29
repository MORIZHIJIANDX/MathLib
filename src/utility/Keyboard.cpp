#include "Keyboard.h"

namespace Dash
{
	Keyboard::Keyboard()
	{
	}

	Keyboard::~Keyboard()
	{
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

	void Keyboard::OnKeyPressed(KeyCode key)
	{
		mKeyStates[static_cast<unsigned int>(key)] = true;
	}

	void Keyboard::OnKeyReleased(KeyCode key)
	{
		mKeyStates[static_cast<unsigned int>(key)] = false;
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
}