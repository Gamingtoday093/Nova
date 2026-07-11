#include "pch.h"
#include "ConsoleTab.h"

#define IMRGB(r, g, b) ImVec4{ float(r) / 256.f, float(g) / 256.f, float(b) / 256.f, 1.f }

ConsoleTab::ConsoleTab() : m_SmallFormatter("[%Y-%m-%d %H:%M:%S.%e] [%n]")
{
	// Constant strings never go out of Scope
	m_LevelNames[spdlog::level::trace] = "Trace";
	m_LevelNames[spdlog::level::debug] = "Debug";
	m_LevelNames[spdlog::level::info] = "Info";
	m_LevelNames[spdlog::level::warn] = "Warning";
	m_LevelNames[spdlog::level::err] = "Error";
	m_LevelNames[spdlog::level::critical] = "Critical";
	m_LevelNames[spdlog::level::off] = "Off";

	// Colors Match Windows Campbell Console Style / spdlog::wincolor_sink
	m_LevelColors[spdlog::level::trace] = { 1, 1, 1, 1 }; // White
	m_LevelColors[spdlog::level::debug] = IMRGB(58, 150, 221); // Cyan
	m_LevelColors[spdlog::level::info] = IMRGB(19, 161, 14); // Green
	m_LevelColors[spdlog::level::warn] = IMRGB(249, 241, 165); // Yellow
	m_LevelColors[spdlog::level::err] = IMRGB(231, 72, 86); // Red
	m_LevelColors[spdlog::level::critical] = { 1, 1, 1, 1 }; // White (w/ Red Background)
	m_LevelColors[spdlog::level::off] = { 0, 0, 0, 0 }; // Transparent

	ImFontConfig config;
	config.SizePixels = 11.f;

	m_SmallFont = ImGui::GetIO().Fonts->AddFontDefault(&config);
}

void ConsoleTab::Render()
{
	ImGui::Begin("Console");

	auto logMsgs = Nova::Log::GetLogMessages();
	for (auto& msg : logMsgs)
	{
		ImGui::Text("[");
		ImGui::SameLine(0, 0);
		if (msg.level == spdlog::level::critical)
			ImGui::TextColoredBg(m_LevelColors[msg.level], IMRGB(231, 72, 86), m_LevelNames[msg.level]);
		else
			ImGui::TextColored(m_LevelColors[msg.level], m_LevelNames[msg.level]);
		ImGui::SameLine(0, 0);
		ImGui::Text("] ");
		ImGui::SameLine(0, 0);
		ImGui::TextUnformatted(msg.payload.begin(), msg.payload.end());
		
		ImGui::PushFont(m_SmallFont);
		spdlog::memory_buf_t smallFormatted;
		m_SmallFormatter.format(msg, smallFormatted);
		ImGui::TextDisabled(SPDLOG_BUF_TO_STRING(smallFormatted).c_str());
		ImGui::PopFont();

		if (&msg != &logMsgs.back()) ImGui::Spacing();
	}

	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::End();
}
