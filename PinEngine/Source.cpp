#include "PinEngine//IncludeMe.h"
#include "PinEngine//Core//Graphics//Generators//SceneGenerator.h"
#include "PinEngine//Core//Graphics//Helpers//SpritesheetGenerator.h"

using namespace PinEngine;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	Window window;
	WindowStyle style = (WindowStyle)(WindowStyle::ExitButton | WindowStyle::Resizable);
	if (window.Initialize(hInstance, L"Test Title", L"Test Class", 800, 600, -1, -1, style))
	{
		//bool result = window.SetWindowColorKey(RGB(255, 255, 255));

		window.renderer.SetActiveScene(SceneGenerator::GenerateTestScene());

		while (window.ProcessMessages())
		{
			window.Frame();
			window.renderer.RenderFrame();
		}
	}
	return 0;
}