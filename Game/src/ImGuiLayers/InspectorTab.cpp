#include "pch.h"
#include "InspectorTab.h"
#include "EditorContext.h"
#include <Nova/Scene/Component/Components.h>
#include <ImGui/ImGui.h>

InspectorTab::InspectorTab(EditorContext& context) : m_Context(context) { }

void InspectorTab::Render()
{
	ImGui::Begin("Inspector");

	if (!m_Context.SelectedEntity)
	{
		ImGui::End();
		return;
	}
	
	if (auto* tagComponent = m_Context.SelectedEntity.TryGetComponent<Nova::TagComponent>())
	{
		ImGui::SeparatorText("Tag");

		ImGui::Text("Name");
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		char nameBuffer[64];
		strncpy_s(nameBuffer, sizeof(nameBuffer), tagComponent->Name.c_str(), tagComponent->Name.length());
		nameBuffer[sizeof(nameBuffer) - 1] = '\0';
		
		if (ImGui::InputText("##TagName", nameBuffer, sizeof(nameBuffer)) && nameBuffer[0] != '\0')
			tagComponent->Name = nameBuffer;
	}

	ImGui::End();
}
