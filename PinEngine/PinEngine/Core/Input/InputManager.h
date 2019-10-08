#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include <memory>

namespace PinEngine
{
	class InputManager
	{
	public:
		static std::shared_ptr<Mouse> GetMouse();
		static std::shared_ptr<Keyboard> GetKeyboard();
		static void RegisterMouse(std::shared_ptr<Mouse> mouse);
		static void RegisterKeyboard(std::shared_ptr<Keyboard> keyboard);
	private:
		static std::shared_ptr<Mouse> mouse;
		static std::shared_ptr<Keyboard> keyboard;
	};
}