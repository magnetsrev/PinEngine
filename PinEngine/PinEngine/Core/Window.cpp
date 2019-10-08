#include "Window.h"
#include "Graphics/ResourceManager.h"
#include "Input/InputManager.h"
#include "Graphics/Generators/SceneGenerator.h"
namespace PinEngine
{
	bool Window::Initialize(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height, int startupX, int startupY, WindowStyle style)
	{
		mouse = std::make_shared<Mouse>();
		InputManager::RegisterMouse(mouse);
		keyboard = std::make_shared<Keyboard>();
		InputManager::RegisterKeyboard(keyboard);

		static bool raw_input_initialized = false;
		if (raw_input_initialized == false)
		{
			try
			{
				RAWINPUTDEVICE rid;

				rid.usUsagePage = 0x01; //Mouse
				rid.usUsage = 0x02;
				rid.dwFlags = 0;
				rid.hwndTarget = NULL;

				if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
				{
					ErrorLogger::Log(L"Failed to register raw input devices.");
					return false;
				}

				raw_input_initialized = true;
			}
			catch (COMException& exception)
			{
				MessageBoxW(NULL, L"Exception", exception.what(), MB_ICONERROR);
				return false;
			}
		}

		this->hInstance = hInstance;
		this->width = width;
		this->height = height;
		PipelineManager::SetWidth(width);
		PipelineManager::SetHeight(height);
		this->window_title = window_title;
		this->window_class = window_class; //wide string representation of class string (used for registering class and creating window)

		RegisterWindowClass();

		int startX = 0;
		int startY = 0;
		if (startupX == -1)
		{
			int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
			startX = centerScreenX;
		}

		if (startupY == -1)
		{
			int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
			startY = centerScreenY;
		}

		RECT wr; //Widow Rectangle
		wr.left = startX;
		wr.top = startY;
		wr.right = wr.left + width;
		wr.bottom = wr.top + height;
		//UINT windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

		

		DWORD windowStyle = 0;
		windowStyle |= WS_POPUP;

		if (style & WindowStyle::ExitButton)
		{
			windowStyle |= WS_SYSMENU | WS_CAPTION;
		}
		if (style & WindowStyle::MinimizeAvailable)
		{
			windowStyle |= WS_SYSMENU | WS_CAPTION| WS_MINIMIZEBOX;
		}
		if (style & WindowStyle::MaximizeAvailable)
		{
			windowStyle |= WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX;
		}
		if (style & WindowStyle::Resizable)
		{
			windowStyle |= WS_SIZEBOX;
		}
		if (style & WindowStyle::NoBorder)
		{
			UINT styleFix = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
			styleFix = ~styleFix;
			windowStyle &= styleFix;
		}

		BOOL result = AdjustWindowRect(&wr, windowStyle, FALSE);

		DWORD extendedWindowsStyle= 0;
		if (style & WindowStyle::TransparencyAllowed)
		{
			extendedWindowsStyle |= WS_EX_LAYERED;
		}
		if (style & WindowStyle::Topmost)
		{
			extendedWindowsStyle |= WS_EX_TOPMOST;
		}

		handle = CreateWindowEx(extendedWindowsStyle, //Extended Windows style - we are using the default. For other options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx
			window_class.c_str(), //Window class name
			window_title.c_str(), //Window Title
			windowStyle, //Windows style - See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
			wr.left, //Window X Position
			wr.top, //Window Y Position
			wr.right - wr.left, //Window Width
			wr.bottom - wr.top, //Window Height
			NULL, //Handle to parent of this window. Since this is the first window, it has no parent window.
			NULL, //Handle to menu or child window identifier. Can be set to NULL and use menu in WindowClassEx if a menu is desired to be used.
			hInstance, //Handle to the instance of module to be used with this window
			this); //Param to create window

		if (handle == NULL)
		{
			ErrorLogger::Log(GetLastError(), L"CreateWindowEX Failed for window: " + window_title);
			return false;
		}

		if (style & WindowStyle::TransparencyAllowed)
		{
			SetWindowAlpha(1.0f);
		}

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(handle, SW_SHOW);
		SetForegroundWindow(handle);
		SetFocus(handle);

		if (!renderer.Initialize(this))
		{

		}

		return true;
	}

	bool Window::ProcessMessages()
	{
		// Handle the windows messages.
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

		while (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
			handle, //Handle to window we are checking messages for
			0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
			0,    //Maximum Filter Msg Value
			PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
		{
			TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
			DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
		}

		// Check if the window was closed
		if (msg.message == WM_NULL)
		{
			if (!IsWindow(handle))
			{
				handle = NULL; //Message processing loop takes care of destroying this window
				UnregisterClass(window_class.c_str(), hInstance);
				return false;
			}
		}

		return true;
	}

	HWND Window::GetHWND() const
	{
		return handle;
	}

	Window::~Window()
	{
		if (handle != NULL)
		{
			UnregisterClass(window_class.c_str(), hInstance);
			DestroyWindow(handle);
		}
	}

	bool Window::SetWindowAlpha(float alpha)
	{
		alpha = std::max(0.0f, alpha);
		alpha = std::min(1.0f, alpha);
		BYTE alphaByte = 255 * alpha;
		this->alpha = alphaByte;
		alphaUsed = true;
		if (colorRefUsed)
		{
			return SetLayeredWindowAttributes(handle, colorRef, alphaByte, LWA_ALPHA | LWA_COLORKEY);
		}
		else
		{
			return SetLayeredWindowAttributes(handle, NULL, alphaByte, LWA_ALPHA);

		}
	}

	bool Window::SetWindowColorKey(COLORREF colorKey)
	{
		this->colorRef = colorKey;
		colorRefUsed = true;
		if (alphaUsed)
		{
			return SetLayeredWindowAttributes(handle, colorKey, alpha, LWA_ALPHA | LWA_COLORKEY);
		}
		else
		{
			return SetLayeredWindowAttributes(handle, colorKey, 0.0f, LWA_COLORKEY);
		}
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			//Keyboard Messages
		case WM_KEYDOWN:
		{
			unsigned char keycode = static_cast<unsigned char>(wParam);
			if (keyboard->IsKeysAutoRepeat())
			{
				keyboard->OnKeyPressed(keycode);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					keyboard->OnKeyPressed(keycode);
				}
			}
			return 0;
		}
		case WM_KEYUP:
		{
			unsigned char keycode = static_cast<unsigned char>(wParam);
			keyboard->OnKeyReleased(keycode);
			return 0;
		}
		case WM_CHAR:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);
			if (keyboard->IsCharsAutoRepeat())
			{
				keyboard->OnChar(ch);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					keyboard->OnChar(ch);
				}
			}
			return 0;
		}
		//Mouse Messages
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse->OnMouseMove(x, y);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse->OnLeftPressed(x, y);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse->OnRightPressed(x, y);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse->OnMiddlePressed(x, y);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse->OnLeftReleased(x, y);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse->OnRightReleased(x, y);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mouse->OnMiddleReleased(x, y);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse->OnWheelUp(x, y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				mouse->OnWheelDown(x, y);
			}
			return 0;
		}
		case WM_INPUT:
		{
			UINT dataSize = 0;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

			if (dataSize > 0)
			{
				std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						mouse->OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					}
				}
			}

			return DefWindowProc(hwnd, uMsg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages
		}
		case WM_SIZE:
		{
			INT width = LOWORD(lParam);
			INT height = HIWORD(lParam);
			renderer.Resize(width, height);
			
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	bool Window::ToggleClickthrough(bool clickthrough)
	{
		LONG currentStyle = GetWindowLongPtr(handle, GWL_EXSTYLE);
		if (clickthrough) //Enable click through
		{
			currentStyle |= WS_EX_TRANSPARENT;
		}
		else //Disable click through
		{
			LONG removeStyle = ~WS_EX_TRANSPARENT;
			currentStyle &= removeStyle;
		}
		if (SetWindowLongPtr(handle, GWL_EXSTYLE, currentStyle) == 0)
			return false;
		return true;
	}

	bool Window::Frame()
	{
		while (!mouse->EventBufferIsEmpty())
		{
			std::shared_ptr<Scene> scene = renderer.GetActiveScene();
			MouseEvent mouseEvent = mouse->ReadEvent();
			if (scene)
			{
				for (const auto& obj : scene->Get2DObjects())
				{
					obj->ProcessMouseEvent(mouseEvent);
				}
			}
			if (mouseEvent.GetType() == MouseEvent::LPress)
			{
				OutputDebugString(L"LPRESS");
			}
		}
		while (!keyboard->KeyBufferIsEmpty())
		{
			KeyboardEvent event = keyboard->ReadKey();
			if (event.GetKeyCode() == VK_ESCAPE)
			{
				SetWindowAlpha(0.4f);
				ToggleClickthrough(true);
			}
		}
		while (!keyboard->CharBufferIsEmpty())
		{
			unsigned char ch = keyboard->ReadChar();
		}

		if (GetAsyncKeyState('A'))
		{
			SetWindowAlpha(0.8f);
			ToggleClickthrough(false);
		}
		return true;
	}

	LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			// All other messages
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;

		default:
		{
			// retrieve ptr to window class
			Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			// forward message to window class handler
			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
		}
	}

	LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_NCCREATE:
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
			if (pWindow == nullptr) //Sanity check
			{
				ErrorLogger::Log(L"Critical Error: Pointer to window container is null during WM_NCCREATE.");
				exit(-1);
			}
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	void Window::RegisterWindowClass()
	{
		WNDCLASSEX wc; //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
		wc.lpfnWndProc = HandleMessageSetup; //Pointer to Window Proc function for handling messages from this window
		wc.cbClsExtra = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
		wc.cbWndExtra = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
		wc.hInstance = hInstance; //Handle to the instance that contains the Window Procedure
		wc.hIcon = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
		wc.hIconSm = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
		wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
		wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
		wc.lpszMenuName = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
		wc.lpszClassName = window_class.c_str(); //Pointer to null terminated string of our class name for this window.
		wc.cbSize = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize
		RegisterClassEx(&wc); // Register the class so that it is usable.
	}
}