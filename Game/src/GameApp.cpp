#include "pch.h"
#include <Nova/EntryPoint.h>
#include <Nova/Application/WindowApplication.h>
#include <Nova/ImGui/ImGuiManager.h>
#include "ImGuiLayers/SceneTab.h"

class GameApp : public Nova::WindowApplication
{
public:
	GameApp(const Nova::ApplicationStartupInfo& startupInfo) : Nova::WindowApplication(startupInfo) { }

	void OnStart() override
	{
		Nova::ImGuiManager::AddLayer(std::make_unique<SceneTab>());
	}

	void OnUpdate() override
	{
		
	}

	void OnShutdown() override
	{

	}
};

Nova::IApplication* Nova::CreateApplication()
{
	Nova::ApplicationStartupInfo startupInfo{};
	startupInfo.Name = "Game";

	return new GameApp(startupInfo);
}