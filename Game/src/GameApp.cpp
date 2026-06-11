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
	}

	void OnUpdate() override
	{
		
	}

	void OnShutdown() override
	{

	}

private:
	EditorContext m_Context;
};

Nova::IApplication* Nova::CreateApplication()
{
	Nova::ApplicationStartupInfo startupInfo{};
	startupInfo.Name = "Game";

	return new GameApp(startupInfo);
}