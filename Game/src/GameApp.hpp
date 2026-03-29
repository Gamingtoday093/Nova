#include <Nova/EntryPoint.hpp>
#include <iostream>

class GameApp : public Nova::IApplication
{
	void Run() override
	{
		std::cout << "Hello World!";
		while (true)
		{

		}
	}
};

Nova::IApplication* Nova::CreateApplication()
{
	return new GameApp();
}