#pragma once
#include <bitset>
#include "InputKeys.h"

namespace Nova
{
	class Input
	{
	public:
		Input();
		~Input();

		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		void BeginFrame();
		static bool UpdateStatesWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

		static bool ShouldProcessInput();
		static void OverrideProcessInputThisFrame();

		static bool KeyDown(EKey key);
		static bool KeyUp(EKey key);
		static bool KeyHeld(EKey key);

		static bool KeyDown(EMouseButton mouseButton);
		static bool KeyUp(EMouseButton mouseButton);
		static bool KeyHeld(EMouseButton mouseButton);

		static POINT GetMousePosition();
		static POINT GetMouseDelta();

		static short GetScrollDelta();

	private:
		inline static Input& Get()
		{
			NOVA_ASSERT(m_Instance, "Input hasn't been Initialized");
			return *m_Instance;
		}

		static constexpr size_t LEFT_MOUSE_BUTTON = size_t(EMouseButton::LEFT);
		static constexpr size_t RIGHT_MOUSE_BUTTON = size_t(EMouseButton::RIGHT);
		static constexpr size_t MIDDLE_MOUSE_BUTTON = size_t(EMouseButton::MIDDLE);

		std::bitset<256> m_PendingState = {};
		std::bitset<256> m_CurrentState = {};
		std::bitset<256> m_PreviousState = {};

		POINT m_PendingMousePosition = {};
		POINT m_CurrentMousePosition = {};
		POINT m_PreviousMousePosition = {};
		
		short m_PendingScrollDelta = 0;
		short m_CurrentScrollDelta = 0;
		short m_PreviousScrollDelta = 0;

		bool m_OverrideProcessInputsThisFrame = false;

		static Input* m_Instance;
	};
}
