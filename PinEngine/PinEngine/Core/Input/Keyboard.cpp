#include "Keyboard.h"

namespace PinEngine
{
	Keyboard::Keyboard()
	{
	}

	bool Keyboard::KeyIsPressed(const unsigned char keycode)
	{
		return keyStates[keycode];
	}

	bool Keyboard::KeyBufferIsEmpty()
	{
		return keyBuffer.empty();
	}

	bool Keyboard::CharBufferIsEmpty()
	{
		return charBuffer.empty();
	}

	KeyboardEvent Keyboard::ReadKey()
	{
		if (keyBuffer.empty()) //If no keys to be read?
		{
			return KeyboardEvent(); //return empty keyboard event
		}
		else
		{
			KeyboardEvent e = keyBuffer.front(); //Get first Keyboard Event from queue
			keyBuffer.pop(); //Remove first item from queue
			return e; //Returns keyboard event
		}
	}

	unsigned char Keyboard::ReadChar()
	{
		if (charBuffer.empty()) //If no keys to be read?
		{
			return 0u; //return 0 (NULL char)
		}
		else
		{
			unsigned char e = charBuffer.front(); //Get first char from queue
			charBuffer.pop(); //Remove first char from queue
			return e; //Returns char
		}
	}

	void Keyboard::OnKeyPressed(const unsigned char key)
	{
		keyStates[key] = true;
		keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
	}

	void Keyboard::OnKeyReleased(const unsigned char key)
	{
		keyStates[key] = false;
		keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
	}

	void Keyboard::OnChar(const unsigned char key)
	{
		charBuffer.push(key);
	}

	void Keyboard::EnableAutoRepeatKeys()
	{
		autoRepeatKeys = true;
	}

	void Keyboard::DisableAutoRepeatKeys()
	{
		autoRepeatKeys = false;
	}

	void Keyboard::EnableAutoRepeatChars()
	{
		autoRepeatChars = true;
	}

	void Keyboard::DisableAutoRepeatChars()
	{
		autoRepeatChars = false;
	}

	bool Keyboard::IsKeysAutoRepeat()
	{
		return autoRepeatKeys;
	}

	bool Keyboard::IsCharsAutoRepeat()
	{
		return autoRepeatChars;
	}
}