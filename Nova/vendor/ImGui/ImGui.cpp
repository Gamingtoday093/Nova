#include "novapch.h"
#include "ImGui.h"
#include <imgui_internal.h>

bool ImGui::SeparatorTextCheckbox(const char* label, const char* checkboxId, bool* v)
{
	ImGui::SeparatorText(label);
	auto& vtxBuffer = ImGui::GetWindowDrawList()->VtxBuffer;
	float lastX = vtxBuffer.back().pos.x + 30.f;
	for (int i = vtxBuffer.size() - 2; i >= 0; i--)
	{
		if (vtxBuffer[i].pos.x < lastX) continue;

		vtxBuffer[i].pos.x -= 30;
		vtxBuffer[i - 1].pos.x -= 30;
		break;
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - 30);
	return ImGui::Checkbox(checkboxId, v);
}

void ImGui::TextColoredBg(const ImVec4& col, const ImVec4& bg, const char* text)
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 textSize = ImGui::CalcTextSize(text);

	ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + textSize.x, pos.y + textSize.y), ImGui::GetColorU32(bg));
	ImGui::SetCursorScreenPos(pos);

	ImGui::TextColored(col, text);
}

ImGuiID ImGui::SetTreeNodeIsOpen(const char* label, ImGuiID seed, bool newIsOpen)
{
	ImGuiID id = ImGui::GetIDWithSeed(label, nullptr, seed);
	ImGui::TreeNodeSetOpen(id, newIsOpen);
	return id;
}

void ImGui::TextUnformatted(const std::string& text)
{
	ImGui::TextUnformatted(text.c_str(), text.c_str() + text.size());
}
