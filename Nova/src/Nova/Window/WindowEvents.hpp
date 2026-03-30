#pragma once
#include <cstdint>

namespace Nova
{
	enum class EWindowEvent
	{
		WindowResize
	};

	class WindowEvent
	{
	public:
		virtual EWindowEvent GetEventType() const = 0;
	};

	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) : Width(width), Height(height) { }

		EWindowEvent GetEventType() const override
		{
			return EWindowEvent::WindowResize;
		}

		uint32_t Width, Height;
	};
}
