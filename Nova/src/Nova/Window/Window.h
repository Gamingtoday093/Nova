#pragma once
#include "WindowEvents.hpp"
#include <functional>
#include <windows.h>

namespace Nova
{
	struct WindowStartupInfo
	{
		std::string Title;

		int32_t Width = CW_USEDEFAULT, Height = CW_USEDEFAULT;
	};

	class Window
	{
	public:
		using WindowEventCallback = std::function<void(WindowEvent&)>;

		Window(const WindowStartupInfo& startupInfo);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool ProcessMessages() const;
		HWND GetHWND() const;
		
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		void SetWindowEventCallback(const WindowEventCallback& callback);

	private:
		void CreateWindowClass(LPCWSTR className);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		HINSTANCE m_HInstance;
		HWND m_HWND;
		WindowEventCallback m_EventCallback;
	};
}

