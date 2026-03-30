#include <Nova/EntryPoint.hpp>
#include <Nova/Application/WindowApplication.h>
#include <iostream>

class GameApp : public Nova::WindowApplication
{
public:
	GameApp(const Nova::ApplicationStartupInfo& startupInfo) : Nova::WindowApplication(startupInfo) { }

	void Run() override
	{
		NOVA_CORE_INFO("Hello World! {0}", 39.9f);
		while (true)
		{
			
		}
	}
};

Nova::IApplication* Nova::CreateApplication()
{
	Nova::ApplicationStartupInfo startupInfo{};
	startupInfo.Name = "Game";

	return new GameApp(startupInfo);
}