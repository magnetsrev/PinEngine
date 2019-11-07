#pragma once
#include <string>
#include "..//Common/ErrorLogger.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include <memory>
#include "Graphics/Renderer.h"

namespace PinEngine
{
	enum WindowStyle : DWORD
	{
		Resizable = (1 << 0),
		ExitButton = (1 << 1),
		MinimizeAvailable = (1 << 2),
		MaximizeAvailable = (1 << 3),
		NoBorder = (1 << 4),
		TransparencyAllowed = (1 << 5),
		Topmost = (1 << 6)
	};

	class Window
	{
		friend class Renderer;
	public:
		bool Initialize(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width = 800, int height = 600, int startupX = -1, int startupY = -1, WindowStyle style = WindowStyle::Resizable);
		bool ProcessMessages();
		HWND GetHWND() const;
		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool ToggleClickthrough(bool clickthrough);

		bool Frame();

		~Window();
		Renderer renderer;

		bool SetWindowAlpha(float alpha);
		bool SetWindowColorKey(COLORREF colorKey);
	protected:
		std::shared_ptr<Keyboard> keyboard;
		std::shared_ptr<Mouse> mouse;
	private:
		void RegisterWindowClass();
		HWND handle = NULL; //Handle to this window
		HINSTANCE hInstance = NULL; //Handle to application instance
		std::wstring window_title = L""; //Wide string representation of window title
		std::wstring window_class = L""; //Wide string representation of window class name
		int width = 0;
		int height = 0;

		bool colorRefUsed = false;
		bool alphaUsed = false;
		BYTE alpha = 255;
		COLORREF colorRef = RGB(0, 0, 0);
	};
}