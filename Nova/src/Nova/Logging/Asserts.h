#pragma once
#include "Log.h"
#include "Nova/Tools/WStringExtensions.h"

#ifdef _MSC_VER
#define NOVA_DEBUGBREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
#define NOVA_DEBUGBREAK() __builtin_trap()
#endif

namespace Nova::Asserts
{
	constexpr std::string_view PROJECT_ROOT = "Nova\\src\\";

	inline consteval std::string_view TrimPath(std::string_view fileFullPath, std::string_view rootPath)
	{
		size_t pos = fileFullPath.rfind(rootPath);
		return pos != std::string_view::npos ? fileFullPath.substr(pos + rootPath.size()) : fileFullPath;
	}
}

#define __PRJ_FILE__ Nova::Asserts::TrimPath(__FILE__, Nova::Asserts::PROJECT_ROOT)

#ifdef DEBUG
#define NOVA_ASSERT(condition, ...)												\
if (!(condition)) [[unlikely]]													\
{																				\
	NOVA_CORE_ERROR(__VA_ARGS__);												\
	if (IsDebuggerPresent())													\
		NOVA_DEBUGBREAK();														\
	else																		\
	{																			\
		MessageBox(nullptr,														\
			ToWString(															\
				std::format(__VA_ARGS__) +										\
				std::format("\nFile: {} on Line: {}", __PRJ_FILE__, __LINE__)	\
			).c_str(),															\
			L"Nova Assertion Failed", MB_OK | MB_ICONERROR);					\
		std::abort();															\
	}																			\
}
#else // Release
#define NOVA_ASSERT(condition, ...) ((void)0)
#endif
