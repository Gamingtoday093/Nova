#include "pch.h"
#include "Window.h"
#include "../../../resources/Icon/resource.h"

Nova::Window::Window(const WindowStartupInfo& startupInfo) : m_HInstance(GetModuleHandle(nullptr))
{
	std::wstring title = ToWString(startupInfo.Title);

	LPCWSTR className = L"Nova";
	CreateWindowClass(className);

	m_HWND = CreateWindowEx(
		0,
		className,
		title.c_str(),
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT, CW_USEDEFAULT, startupInfo.Width, startupInfo.Height,

		NULL,
		NULL,
		m_HInstance,
		NULL
	);

	NOVA_ASSERT(m_HWND != NULL, "Failed to Create Window");	

	SetWindowLongPtr(m_HWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ShowWindow(m_HWND, SW_SHOW);
}

Nova::Window::~Window()
{
	DestroyWindow(m_HWND);
}

bool Nova::Window::ProcessMessages() const
{
	MSG msg{};

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

HWND Nova::Window::GetHWND() const
{
	return m_HWND;
}

uint32_t Nova::Window::GetWidth() const
{
	RECT rect;
	GetClientRect(m_HWND, &rect);

	return rect.right - rect.left;
}

uint32_t Nova::Window::GetHeight() const
{
	RECT rect;
	GetClientRect(m_HWND, &rect);

	return rect.bottom - rect.top;
}

void Nova::Window::SetWindowEventCallback(const WindowEventCallback& callback)
{
	m_EventCallback = callback;
}

void Nova::Window::CreateWindowClass(LPCWSTR className)
{
	WNDCLASS windowClass
	{
		.lpfnWndProc = WindowProc,
		.hInstance = m_HInstance,
		.hIcon = LoadIcon(m_HInstance, MAKEINTRESOURCE(IDI_ICON2)),
		.lpszClassName = className
	};

	RegisterClass(&windowClass);
}

LRESULT Nova::Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Nova::Window* window = reinterpret_cast<Nova::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_SIZE:
			if (window && window->m_EventCallback)
			{
				WindowResizeEvent resizeEvent(LOWORD(lParam), HIWORD(lParam));
				window->m_EventCallback(resizeEvent);
			}
			return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));

			EndPaint(hwnd, &ps);
		}
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
