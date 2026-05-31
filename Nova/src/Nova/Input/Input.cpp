#include "novapch.h"
#include "Input.h"
#include <ImGui/ImGui.h>

Nova::Input* Nova::Input::m_Instance;

Nova::Input::Input()
{
	NOVA_ASSERT(!m_Instance, "Creating multiple Input is not allowed");
	m_Instance = this;
}

Nova::Input::~Input()
{
	m_Instance = nullptr;
}

void Nova::Input::BeginFrame()
{
	// Keyboard & Mouse Buttons
	m_PreviousState = m_CurrentState;
	m_CurrentState = m_PendingState;

	// Mouse Position
	m_PreviousMousePosition = m_CurrentMousePosition;
	m_CurrentMousePosition = m_PendingMousePosition;

	// Mouse Scrollwheel
	m_PreviousScrollDelta = m_CurrentScrollDelta;
	m_CurrentScrollDelta = m_PendingScrollDelta;
	m_PendingScrollDelta = 0;
}

bool Nova::Input::UpdateStatesWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Keyboard
		case WM_KEYDOWN:
			Input::Get().m_PendingState[wParam] = true;
			return true;
		case WM_KEYUP:
			Input::Get().m_PendingState[wParam] = false;
			return true;

		// Mouse Buttons
		case WM_LBUTTONDOWN:
			Input::Get().m_PendingState[LEFT_MOUSE_BUTTON] = true;
			return true;
		case WM_LBUTTONUP:
			Input::Get().m_PendingState[LEFT_MOUSE_BUTTON] = false;
			return true;
		case WM_RBUTTONDOWN:
			Input::Get().m_PendingState[RIGHT_MOUSE_BUTTON] = true;
			return true;
		case WM_RBUTTONUP:
			Input::Get().m_PendingState[RIGHT_MOUSE_BUTTON] = false;
			return true;
		case WM_MBUTTONDOWN:
			Input::Get().m_PendingState[MIDDLE_MOUSE_BUTTON] = true;
			return true;
		case WM_MBUTTONUP:
			Input::Get().m_PendingState[MIDDLE_MOUSE_BUTTON] = false;
			return true;

		// Mouse Position
		case WM_MOUSEMOVE:
		{
			Input& input = Input::Get();
			input.m_PendingMousePosition.x = LOWORD(lParam);
			input.m_PendingMousePosition.y = HIWORD(lParam);
			return true;
		}

		// Mouse Scrollwheel
		case WM_MOUSEWHEEL:
			Input::Get().m_PendingScrollDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			return true;
	}
	return false;
}

bool Nova::Input::ShouldProcessInput()
{
	auto& io = ImGui::GetIO();
	return !io.WantCaptureMouse && !io.WantTextInput;
}

bool Nova::Input::KeyDown(EKey key)
{
	if (!ShouldProcessInput()) return false;

	Input& input = Input::Get();
	return input.m_CurrentState[size_t(key)] && !input.m_PreviousState[size_t(key)];
}

bool Nova::Input::KeyUp(EKey key)
{
	if (!ShouldProcessInput()) return false;

	Input& input = Input::Get();
	return !input.m_CurrentState[size_t(key)] && input.m_PreviousState[size_t(key)];
}

bool Nova::Input::KeyHeld(EKey key)
{
	if (!ShouldProcessInput()) return false;

	Input& input = Input::Get();
	return input.m_CurrentState[size_t(key)] && input.m_PreviousState[size_t(key)];
}

bool Nova::Input::KeyDown(EMouseButton mouseButton)
{
	return KeyDown(EKey(mouseButton));
}

bool Nova::Input::KeyUp(EMouseButton mouseButton)
{
	return KeyUp(EKey(mouseButton));
}

bool Nova::Input::KeyHeld(EMouseButton mouseButton)
{
	return KeyHeld(EKey(mouseButton));
}

POINT Nova::Input::GetMousePosition()
{
	return Input::Get().m_CurrentMousePosition;
}

POINT Nova::Input::GetMouseDelta()
{
	Input& input = Input::Get();
	POINT delta
	{
		.x = input.m_CurrentMousePosition.x - input.m_PreviousMousePosition.x,
		.y = input.m_CurrentMousePosition.y - input.m_PreviousMousePosition.y
	};
	return delta;
}

short Nova::Input::GetScrollDelta()
{
	if (!ShouldProcessInput()) return 0;

	return -Input::Get().m_CurrentScrollDelta;
}
