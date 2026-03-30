#pragma once

namespace Nova
{
	struct ApplicationStartupInfo
	{
		std::string Name;
	};

	class IApplication
	{
	public:
		IApplication(const ApplicationStartupInfo& startupInfo) { m_StartupInfo = startupInfo; }
		virtual ~IApplication() = default;
		virtual void Run() = 0;

		const ApplicationStartupInfo& GetAppStartupInfo() { return m_StartupInfo; }

	protected:
		ApplicationStartupInfo m_StartupInfo;
	};
}
