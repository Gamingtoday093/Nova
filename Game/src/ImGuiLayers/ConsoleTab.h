#pragma once
#include <Nova/ImGui/IImGuiLayer.hpp>
#include <ImGui/ImGui.h>

class ConsoleTab : public Nova::IImGuiLayer
{
public:
	ConsoleTab();

	void Render() override;

private:
	template<typename T>
	using LevelFormat = std::array<T, spdlog::level::n_levels>;

	LevelFormat<const char*> m_LevelNames;
	LevelFormat<ImVec4> m_LevelColors;

	ImFont* m_SmallFont;
	spdlog::pattern_formatter m_SmallFormatter;
};
