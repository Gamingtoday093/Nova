#include "pch.h"
#include "InspectorTab.h"
#include "EditorContext.h"
#include <ImGui/ImGui.h>
#include <Nova/Scene/Component/Components.h>
#include <Nova/Tools/string_buffer.hpp>

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

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name");
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		Nova::string_buffer<48> nameBuffer = tagComponent->Name;
		if (ImGui::InputText("##TagName", nameBuffer.data(), sizeof(nameBuffer)) && !nameBuffer.empty())
			tagComponent->Name = nameBuffer;
	}

	if (auto* transformComponent = m_Context.SelectedEntity.TryGetComponent<Nova::TransformComponent>())
	{
		ImGui::SeparatorText("Transform");

		if (ImGui::BeginTable("TransformTable", 2))
		{
			ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Position");
			
			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("##TransformPosition", reinterpret_cast<float*>(&transformComponent->Transform.Position));
			
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Rotation");

			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-1);

			constexpr float PI = 3.14159265358979323846f;
			constexpr float RadToDegFactor = 180.f / PI;
			constexpr float DegToRadFactor = PI / 180.f;
			float degreeRotation[3]
			{
				transformComponent->Transform.Rotation.x * RadToDegFactor,
				transformComponent->Transform.Rotation.y * RadToDegFactor,
				transformComponent->Transform.Rotation.z * RadToDegFactor
			};
			if (ImGui::InputFloat3("##TransformRotation", degreeRotation))
			{
				transformComponent->Transform.Rotation.x = degreeRotation[0] * DegToRadFactor;
				transformComponent->Transform.Rotation.y = degreeRotation[1] * DegToRadFactor;
				transformComponent->Transform.Rotation.z = degreeRotation[2] * DegToRadFactor;
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Scale");

			ImGui::TableNextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::InputFloat3("##TransformScale", reinterpret_cast<float*>(&transformComponent->Transform.Scale));

			ImGui::EndTable();
		}
	}

	if (auto* meshRendererComponent = m_Context.SelectedEntity.TryGetComponent<Nova::MeshRendererComponent>())
	{
		ImGui::SeparatorText("Mesh Renderer");

		ImGui::Text("Mesh");
		ImGui::SameLine();
		ImGui::Text(meshRendererComponent->Mesh ? "Yes" : "No");

		ImGui::Text("Material");
		ImGui::SameLine();
		ImGui::Text(meshRendererComponent->Material ? "Yes" : "No");
	}

	if (auto* scriptCollectionComponent = m_Context.SelectedEntity.TryGetComponent<Nova::Scripts::ScriptCollectionComponent>())
	{
		for (auto* script : *scriptCollectionComponent)
		{
			bool enabled = script->GetEnabled();
			if (ImGui::SeparatorTextCheckbox(script->GetScriptName(), (std::string("##") + script->GetScriptName()).c_str(), &enabled))
				script->SetEnabled(enabled);
		}
	}

	ImGui::End();
}
