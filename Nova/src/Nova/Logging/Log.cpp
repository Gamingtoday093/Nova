#include "pch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::unique_ptr<spdlog::logger> Nova::Log::s_CoreLogger;
std::unique_ptr<spdlog::logger> Nova::Log::s_ClientLogger;

void Nova::Log::Initialize(const std::string& clientName)
{
    auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    s_CoreLogger = std::make_unique<spdlog::logger>("NovaCore", sink);
    s_ClientLogger = std::make_unique<spdlog::logger>(clientName, sink);
}

spdlog::logger& Nova::Log::GetCoreLogger()
{
    assert(s_CoreLogger && "Logger hasn't been Initialized");
    return *s_CoreLogger;
}

spdlog::logger& Nova::Log::GetClientLogger()
{
    assert(s_ClientLogger && "Logger hasn't been Initialized");
    return *s_ClientLogger;
}
