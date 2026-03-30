#include "pch.h"
#include "WindowApplication.h"

Nova::WindowApplication::WindowApplication(const ApplicationStartupInfo& startupInfo) : IApplication(startupInfo)
{
	Log::Initialize(m_StartupInfo.Name);
}