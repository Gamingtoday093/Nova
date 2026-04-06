#include <Nova/EntryPoint.hpp>
#include <Nova/Application/WindowApplication.h>

class GameApp : public Nova::WindowApplication
{
public:
	GameApp(const Nova::ApplicationStartupInfo& startupInfo) : Nova::WindowApplication(startupInfo) { }

	void OnStart() override
	{

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