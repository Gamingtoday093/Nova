#pragma once
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace ImGui {
	bool SeparatorTextCheckbox(const char* label, const char* checkboxId, bool* v);
	void TextColoredBg(const ImVec4& col, const ImVec4& bg, const char* text);
}