#include "pch.h"
#include "SceneTab.h"
#include "EditorContext.h"
#include <Nova/Graphics/DX11.h>
#include <Nova/Input/Input.h>
#include <Nova/Scene/Component/Components.h>
#include <Nova/Tools/QuaternionExtensions.h>

SceneTab::SceneTab(EditorContext& context) : m_Context(context)
{
	m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	m_GizmoSpace = ImGuizmo::MODE::WORLD;
}

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

	ImGui::Image(reinterpret_cast<ImTextureID>(m_RenderTexture.GetTexture()), textureSize);

	if (m_Context.Scene && m_Context.SelectedEntity)
		if (auto* transformComponent = m_Context.SelectedEntity.TryGetComponent<Nova::TransformComponent>())
		{
			ImVec2 viewportMin = ImGui::GetItemRectMin();
			ImVec2 viewportMax = ImGui::GetItemRectMax();

			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(viewportMin.x, viewportMin.y, viewportMax.x - viewportMin.x, viewportMax.y - viewportMin.y);

			const Nova::Camera& camera = m_Context.Scene->GetCamera();

			DirectX::XMFLOAT4X4 viewMatrix;
			DirectX::XMStoreFloat4x4(&viewMatrix, camera.GetViewMatrix());

			DirectX::XMFLOAT4X4 projectionMatrix;
			DirectX::XMStoreFloat4x4(
				&projectionMatrix,
				DirectX::XMMatrixPerspectiveFovLH(
					camera.FovAngle,
					m_RenderTexture.GetAspectRatio(),
					camera.NearClipPlane, camera.FarClipPlane)
			);

			DirectX::XMFLOAT4X4 modelMatrix;
			DirectX::XMStoreFloat4x4(&modelMatrix, transformComponent->Transform.CalculateMatrix());

			float snap[3]
			{
				15.f,
				15.f,
				15.f
			};
			if (ImGuizmo::Manipulate(&viewMatrix(0, 0), &projectionMatrix(0, 0), m_GizmoOperation, m_GizmoSpace, &modelMatrix(0, 0), nullptr, ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ? snap : nullptr))
			{
				DirectX::XMVECTOR position;
				DirectX::XMVECTOR rotation;
				DirectX::XMVECTOR scale;
				DirectX::XMMatrixDecompose(&scale, &rotation, &position, DirectX::XMLoadFloat4x4(&modelMatrix));

				DirectX::XMStoreFloat3(&transformComponent->Transform.Position, position);
				transformComponent->Transform.Rotation = DirectX::XMQuaternionToEulerAngles(rotation);
				DirectX::XMStoreFloat3(&transformComponent->Transform.Scale, scale);
			}
		}

	if (ImGui::IsItemHovered() && !ImGuizmo::IsUsing())
	{
		if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			if (ImGui::IsKeyDown(ImGuiKey_Q))
				m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
			else if (ImGui::IsKeyDown(ImGuiKey_W))
				m_GizmoOperation = ImGuizmo::OPERATION::ROTATE;
			else if (ImGui::IsKeyDown(ImGuiKey_R))
				m_GizmoOperation = ImGuizmo::OPERATION::SCALE;
			else if (ImGui::IsKeyPressed(ImGuiKey_T))
			{
				if (m_GizmoSpace == ImGuizmo::MODE::WORLD)
					m_GizmoSpace = ImGuizmo::MODE::LOCAL;
				else
					m_GizmoSpace = ImGuizmo::MODE::WORLD;
			}
			else
				// This *works* but not for Multi-Viewports but thats more of an issue with Nova::Input
				Nova::Input::OverrideProcessInputThisFrame();
		}
		else
			Nova::Input::OverrideProcessInputThisFrame();
	}

	ImGui::End();

	ImGui::PopStyleVar();
}

void SceneTab::EndFrame()
{
	Nova::Graphics::DX11::SetRenderTexture(nullptr);
}
