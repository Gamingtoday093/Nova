#include "pch.h"
#include "SceneTab.h"
#include <ImGui/ImGui.h>
#include <Nova/Graphics/DX11.h>
#include <Nova/Input/Input.h>

SceneTab::~SceneTab()
{
	Nova::Graphics::DX11::SetRenderTexture(nullptr);
}

void SceneTab::BeginFrame()
{
	if (m_RenderTexture.GetWidth() != m_Width || m_RenderTexture.GetHeight() != m_Height)
		m_RenderTexture.Resize(m_Width, m_Height);

	Nova::Graphics::DX11::SetRenderTexture(&m_RenderTexture);
}

void SceneTab::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });

	ImGui::Begin("Scene", nullptr, 
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav // Disable all forms of Scrolling
	);

	auto textureSize = ImGui::GetContentRegionAvail();
	m_Width = textureSize.x <= 0 ? 0 : uint32_t(textureSize.x);
	m_Height = textureSize.y <= 0 ? 0 : uint32_t(textureSize.y);

	ImGui::Image((void*)m_RenderTexture.GetTexture(), textureSize);
	
	// This *works* but not for Multi-Viewports but thats more of an issue with Nova::Input
	if (ImGui::IsItemHovered())
		Nova::Input::OverrideProcessInputThisFrame();

	ImGui::End();

	ImGui::PopStyleVar();
}

void SceneTab::EndFrame()
{
	Nova::Graphics::DX11::SetRenderTexture(nullptr);
}
