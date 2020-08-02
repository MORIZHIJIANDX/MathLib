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
		friend class Window;
	public:
		static Keyboard& Get();

		bool IsKeyPressed(KeyCode key) const;

		std::optional<char> ReadChar();

		void EnableAutoRepeat();
		void DisableAutoRepeat();
		bool IsAutoRepeatEnabled() const;

	public:
		KeyboardEvent KeyPressed;
		KeyboardEvent KeyReleased;

	private:
		Keyboard() {};
		~Keyboard() {};

		void OnKeyPressed(KeyEventArgs& e);
		void OnKeyReleased(KeyEventArgs& e);
		void OnChar(char character);

		void FlushCharBuffer();

		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);

		void ClearStates();
	
	private:
		std::bitset<256> mKeyStates;
		std::queue<char> mCharBuffer;
		bool mAutoRepeat = false;
	};
}