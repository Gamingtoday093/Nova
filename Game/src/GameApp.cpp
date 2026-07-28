#include "pch.h"
#include <Nova/EntryPoint.h>
#include <Nova/Application/WindowApplication.h>
#include <Nova/ImGui/ImGuiManager.h>
#include "ImGuiLayers/EditorContext.h"
#include "ImGuiLayers/SceneTab.h"
#include "ImGuiLayers/AssetsTab.h"
#include "ImGuiLayers/HierarchyTab.h"
#include "ImGuiLayers/InspectorTab.h"
#include "ImGuiLayers/ConsoleTab.h"
#include <Nova/Graphics/Renderer/GizmoRenderer.h>
#include <Nova/Navigation/NavGrid.h>
#include <Nova/Scene/Component/Components.h>
#include <Nova/Tools/Stopwatch.h>

class GameApp : public Nova::WindowApplication
{
public:
	GameApp(const Nova::ApplicationStartupInfo& startupInfo) : Nova::WindowApplication(startupInfo) { }

	void OnStart() override
	{
		m_Context.Scene = m_Scene.get();

		Nova::ImGuiManager::AddLayer(std::make_unique<SceneTab>(GetHWND(), m_Context));
		Nova::ImGuiManager::AddLayer(std::make_unique<AssetsTab>());
		Nova::ImGuiManager::AddLayer(std::make_unique<HierarchyTab>(m_Context));
		Nova::ImGuiManager::AddLayer(std::make_unique<InspectorTab>(m_Context));
		Nova::ImGuiManager::AddLayer(std::make_unique<ConsoleTab>());

		m_NavGrid.Resize(256, 256);
		m_NavGrid.NodeSize = 5.5f;
		m_NavGrid.StampSquare({ 20, 20 }, 8, Nova::GridNode::OCCUPIED);
		m_NavGrid.StampSquare({ 19, 18 }, 4, 0);
		m_NavGrid.StampSquare({ 0, -3 }, 9, 20);
		m_NavGrid.StampCircle({ 60, 32 }, 12, 240);
		m_NavGrid.StampCircle({ 50, 28 }, 9, 0);
	}

	void OnUpdate() override
	{
		float halfNodeSize = m_NavGrid.NodeSize * 0.5f;
		float widthX = float(m_NavGrid.GetWidth()) * halfNodeSize;
		float heightZ = float(m_NavGrid.GetHeight()) * halfNodeSize;
		Nova::Graphics::GizmoRenderer::RenderPlane({ widthX, 0.f, heightZ }, { widthX, heightZ });

		auto& nodes = m_NavGrid.GetNodes();
		for (size_t i = 0; i < nodes.size(); i++)
		{
			auto node = nodes[i];
			if (!node.HasCost()) continue;

			XMFLOAT4 color;
			if (!node.IsOccupied())
			{
				DirectX::XMStoreFloat4(&color, DirectX::XMVectorLerp(DirectX::XMVectorSet(1, 1, 1, 0.2f), DirectX::XMVectorSet(0.8f, 0.2f, 0.2f, 0.5f), float(node.Value) / float(Nova::GridNode::OCCUPIED)));
			}
			else
				color = { 1, 0, 0, 1 };
			Nova::Graphics::GizmoRenderer::RenderPlane(m_NavGrid.GetWorldPosition(int32_t(i)), { halfNodeSize, halfNodeSize }, color);
		}

		auto entities = m_Scene->GetAllEntities();
		auto from = DirectX::XMLoadFloat3(&entities[0].GetComponent<Nova::TransformComponent>().Transform.Position);
		auto to = DirectX::XMLoadFloat3(&entities[1].GetComponent<Nova::TransformComponent>().Transform.Position);
		if (m_NavGrid.Pathfind(from, to, m_ResultPath))
		{
			for (size_t i = 0; i < m_ResultPath.size() - 1; i++)
			{
				Nova::Graphics::GizmoRenderer::RenderLine(m_ResultPath[i], m_ResultPath[i + 1], { 0.5f, 0.5f, 1.f, 1.f });
			}
		}
	}

	void OnShutdown() override
	{

	}

private:
	EditorContext m_Context;
	Nova::NavGrid m_NavGrid;
	std::vector<XMFLOAT3> m_ResultPath;
};

Nova::IApplication* Nova::CreateApplication()
{
	Nova::ApplicationStartupInfo startupInfo{};
	startupInfo.Name = "Game";

	return new GameApp(startupInfo);
}