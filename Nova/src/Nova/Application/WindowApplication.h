#pragma once
#include "IApplication.hpp"

namespace Nova
{
	class WindowApplication : public IApplication
	{
	public:
		WindowApplication(const ApplicationStartupInfo& startupInfo);
	};
}

