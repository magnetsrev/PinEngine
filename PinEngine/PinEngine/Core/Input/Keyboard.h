#pragma once
#include "KeyboardEvent.h"
#include <queue>

namespace PinEngine
{
	class Keyboard
	{
		friend class Window;
	public:
		Keyboard();
		bool KeyIsPressed(const unsigned char keycode);
		bool KeyBufferIsEmpty();
		bool CharBufferIsEmpty();
		KeyboardEvent ReadKey();
		unsigned char ReadChar();
		
		void EnableAutoRepeatKeys();
		void DisableAutoRepeatKeys();
		void EnableAutoRepeatChars();
		void DisableAutoRepeatChars();
		bool IsKeysAutoRepeat();
		bool IsCharsAutoRepeat();
	private:
		void OnKeyPressed(const unsigned char key);
		void OnKeyReleased(const unsigned char key);
		void OnChar(const unsigned char key);

		bool autoRepeatKeys = false;
		bool autoRepeatChars = false;
		bool keyStates[256] = { false };
		std::queue<KeyboardEvent> keyBuffer;
		std::queue<unsigned char> charBuffer;
	};
}