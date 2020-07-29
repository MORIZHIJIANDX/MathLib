#pragma once
#include <bitset>
#include <queue>
#include <optional>
#include "KeyCodes.h"
#include "Events.h"

namespace Dash
{
	class Keyboard
	{
	public:
		Keyboard();
		~Keyboard();
	
		bool IsKeyPressed(KeyCode key) const;

		std::optional<char> ReadChar();

		void FlushCharBuffer();

	private:
		void OnKeyPressed(KeyCode key);
		void OnKeyReleased(KeyCode key);
		void OnChar(char character);

		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);

		void ClearStates();
	
	private:
		std::bitset<256> mKeyStates;
		std::queue<char> mCharBuffer;
	};
}