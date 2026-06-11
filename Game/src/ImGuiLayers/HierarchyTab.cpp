#include "pch.h"
#include "HierarchyTab.h"
#include "EditorContext.h"
#include <ImGui/ImGui.h>
#include <Nova/Scene/Component/Components.h>

HierarchyTab::HierarchyTab(EditorContext& context) : m_Context(context) { }

void HierarchyTab::Render()
{
	ImGui::Begin("Hierarchy");

	for (const auto& entity : m_Context.Scene->GetAllEntities())
	{
		auto* tagComponent = entity.TryGetComponent<Nova::TagComponent>();

		std::string tag = tagComponent ? tagComponent->Name : "[Missing Tag]";
		tag += "##" + std::to_string(static_cast<uint32_t>(entity.GetEntityHandle()));

		if (ImGui::Selectable(tag.c_str(), m_Context.SelectedEntity == entity))
		{
			m_Context.SelectedEntity = entity;
		}
	}

	ImGui::End();
}
