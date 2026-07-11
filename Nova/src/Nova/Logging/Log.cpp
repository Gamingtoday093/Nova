#include "novapch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ringbuffer_sink.h>

std::unique_ptr<spdlog::logger> Nova::Log::s_CoreLogger;
std::unique_ptr<spdlog::logger> Nova::Log::s_ClientLogger;
spdlog::sink_ptr Nova::Log::s_LogBuffer;

void Nova::Log::Initialize(const std::string& clientName)
{
    auto stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    s_LogBuffer = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(LOG_MAX_MESSAGES);

    spdlog::sinks_init_list sinks = { stdoutSink, s_LogBuffer };
    s_CoreLogger = std::make_unique<spdlog::logger>("NovaCore", sinks);
    s_ClientLogger = std::make_unique<spdlog::logger>(clientName, sinks);
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

std::vector<spdlog::details::log_msg_buffer> Nova::Log::GetLogMessages()
{
    assert(s_LogBuffer && "Logger hasn't been Initialized");
    return std::static_pointer_cast<spdlog::sinks::ringbuffer_sink_mt>(s_LogBuffer)->last_raw();
}
