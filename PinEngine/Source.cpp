#include "PinEngine/IncludeMe.h"

using namespace PinEngine;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Window window;
	WindowStyle style = (WindowStyle)(WindowStyle::NoBorder);
	if (window.Initialize(hInstance, L"Test Title", L"Test Class", 800, 600, -1, -1, style))
	{
		//window.SetWindowAlpha(0.5f);
		//bool result = window.SetWindowColorKey(RGB(0, 0, 0));
		while (window.ProcessMessages())
		{
			window.Frame();
			window.renderer.RenderFrame();
		}
	}
	return 0;
}