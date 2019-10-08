#include "InputManager.h"

namespace PinEngine
{
	std::shared_ptr<Mouse> InputManager::GetMouse()
	{
		return InputManager::mouse;
	}
	std::shared_ptr<Keyboard> InputManager::GetKeyboard()
	{
		return InputManager::keyboard;
	}
	void InputManager::RegisterMouse(std::shared_ptr<Mouse> mouse)
	{
		InputManager::mouse = mouse;
	}
	void InputManager::RegisterKeyboard(std::shared_ptr<Keyboard> keyboard)
	{
		InputManager::keyboard = keyboard;
	}
	std::shared_ptr<Mouse> InputManager::mouse;
	std::shared_ptr<Keyboard> InputManager::keyboard;
}