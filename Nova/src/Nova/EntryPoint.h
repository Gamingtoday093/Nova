#pragma once
#include "Application/IApplication.hpp"

namespace Nova
{
	extern IApplication* CreateApplication();

	int Create()
	{
		std::unique_ptr<IApplication> app(CreateApplication());
		app->Run();
		return 0;
	}
}

int main()
{
	return Nova::Create();
}
