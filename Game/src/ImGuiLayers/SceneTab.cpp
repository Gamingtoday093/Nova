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

	ImGui::Image(static_cast<ImTextureID>(m_RenderTexture.GetTexture()), textureSize);

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

	m_CaptureMouse &= ImGui::IsMouseDown(ImGuiMouseButton_Right) && !ImGuizmo::IsUsing();
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
			else if (m_Context.Scene && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				ImVec2 mousePos = ImGui::GetMousePos();
				ImVec2 windowPos = ImGui::GetWindowPos();
				ImVec2 windowSize = ImGui::GetWindowSize();

				// Relative position inside of the ImGui Window Viewport where (0, 0) is Top Left and (1, 1) is Bottom Right
				XMVECTOR insideViewportPos =
					XMVectorSet(
						(mousePos.x - windowPos.x) / windowSize.x,
						(mousePos.y - windowPos.y - ImGui::GetFrameHeight()) / windowSize.y,
						0, 0);

				XMVECTOR origin = m_Context.Scene->GetCamera().ScreenToWorld(insideViewportPos);
				XMVECTOR direction = DirectX::XMVector3Normalize(origin - DirectX::XMLoadFloat3(&m_Context.Scene->m_FreeLookCamera.m_Position));

				float closestDistance = FLT_MAX;
				m_Context.SelectedEntity = Nova::Entity::Invalid;
				for (Nova::Entity& entity : m_Context.Scene->GetAllEntities())
				{
					auto* transformComponent = entity.TryGetComponent<Nova::TransformComponent>();
					if (!transformComponent) continue;
					auto* meshRendererComponent = entity.TryGetComponent<Nova::MeshRendererComponent>();
					if (!meshRendererComponent) continue;

					float hitDistance;
					if (meshRendererComponent->GetBounds(transformComponent->Transform).RayBounds(origin, direction, &hitDistance) && hitDistance <= closestDistance)
					{
						closestDistance = hitDistance;
						m_Context.SelectedEntity = entity;
					}
				}
			}
			else
				// This *works* but not for Multi-Viewports but thats more of an issue with Nova::Input
				Nova::Input::OverrideProcessInputThisFrame();
		}
		else // IsMouseDown(ImGuiMouseButton_Right)
		{
			m_CaptureMouse = true;
			Nova::Input::OverrideProcessInputThisFrame();
		}
	}

	if (m_CaptureMouse && !ImGui::IsItemHovered())
	{
		auto& io = ImGui::GetIO();
		ImVec2 windowPos = ImGui::GetWindowPos();
		windowPos = { windowPos.x, windowPos.y + ImGui::GetFrameHeight() };
		ImVec2 windowSize = ImGui::GetWindowSize();
		windowSize = { windowSize.x, windowSize.y - ImGui::GetFrameHeight() };
		
		bool wantSetMousePos = false;
		POINT newMousePos = { int32_t(io.MousePos.x), int32_t(io.MousePos.y) };
		if (io.MousePos.x < windowPos.x)
		{
			wantSetMousePos = true;
			newMousePos = { int32_t(windowPos.x + windowSize.x), newMousePos.y };
		}
		
		if (io.MousePos.y < windowPos.y)
		{
			wantSetMousePos = true;
			newMousePos = { newMousePos.x, int32_t(windowPos.y + windowSize.y) };
		}
		
		if (io.MousePos.x > windowPos.x + windowSize.x)
		{
			wantSetMousePos = true;
			newMousePos = { int32_t(windowPos.x), newMousePos.y };
		}
		
		if (io.MousePos.y > windowPos.y + windowSize.y)
		{
			wantSetMousePos = true;
			newMousePos = { newMousePos.x, int32_t(windowPos.y) };
		}

		if (wantSetMousePos)
		{
			// Use Nova::Input::SetMousePosition instead of ImGuiIO.WantSetMousePos as Scene Camera uses Nova::Input::GetMouseDelta() to Prevent Large jumps
			// This is essentially the exact same, Except Nova::Input Mouse is Relative to the Window and ImGui uses Screen
			ScreenToClient(GetForegroundWindow(), &newMousePos);
			Nova::Input::SetMousePosition(newMousePos);
		}
	}

	ImGui::End();

	ImGui::PopStyleVar();
}

void SceneTab::EndFrame()
{
	Nova::Graphics::DX11::SetRenderTexture(nullptr);
}
