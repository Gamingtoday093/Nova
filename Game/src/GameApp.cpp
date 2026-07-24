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

		Nova::ImGuiManager::AddLayer(std::make_unique<SceneTab>(m_Context));
		Nova::ImGuiManager::AddLayer(std::make_unique<AssetsTab>());
		Nova::ImGuiManager::AddLayer(std::make_unique<HierarchyTab>(m_Context));
		Nova::ImGuiManager::AddLayer(std::make_unique<InspectorTab>(m_Context));
		Nova::ImGuiManager::AddLayer(std::make_unique<ConsoleTab>());

		m_NavGrid.Resize(256, 256);
		m_NavGrid.NodeSize = 2.f;
		m_NavGrid.StampSquare({ 20, 20 }, 8, 200);
		m_NavGrid.StampSquare({ 0, -3 }, 9, 20);
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
			if (!node.IsOccupied()) continue;

			Nova::Graphics::GizmoRenderer::RenderPlane(m_NavGrid.GetWorldPosition(int32_t(i)), { halfNodeSize, halfNodeSize }, { 1, 0, 0, 1 });
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