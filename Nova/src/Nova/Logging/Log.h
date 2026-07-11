#pragma once
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>

namespace Nova
{
	class Log
	{
	public:
		static void Initialize(const std::string& clientName);
		
		static spdlog::logger& GetCoreLogger();
		static spdlog::logger& GetClientLogger();

		static std::vector<spdlog::details::log_msg_buffer> GetLogMessages();

	private:
		static std::unique_ptr<spdlog::logger> s_CoreLogger;
		static std::unique_ptr<spdlog::logger> s_ClientLogger;

		static constexpr size_t LOG_MAX_MESSAGES = 64;
		static spdlog::sink_ptr s_LogBuffer;
	};
}

#ifdef DEBUG
#define NOVA_CORE_INFO(...) Nova::Log::GetCoreLogger().info(__VA_ARGS__)
#define NOVA_CORE_WARN(...) Nova::Log::GetCoreLogger().warn(__VA_ARGS__)
#define NOVA_CORE_ERROR(...) Nova::Log::GetCoreLogger().error(__VA_ARGS__)
#define NOVA_CORE_TRACE(...) Nova::Log::GetCoreLogger().trace(__VA_ARGS__)

#define NOVA_INFO(...) Nova::Log::GetClientLogger().info(__VA_ARGS__)
#define NOVA_WARN(...) Nova::Log::GetClientLogger().warn(__VA_ARGS__)
#define NOVA_ERROR(...) Nova::Log::GetClientLogger().error(__VA_ARGS__)
#define NOVA_TRACE(...) Nova::Log::GetClientLogger().trace(__VA_ARGS__)
#else // Release
#define NOVA_CORE_INFO(...)
#define NOVA_CORE_WARN(...)
#define NOVA_CORE_ERROR(...)
#define NOVA_CORE_TRACE(...)

#define NOVA_INFO(...)
#define NOVA_WARN(...)
#define NOVA_ERROR(...)
#define NOVA_TRACE(...)
#endif
