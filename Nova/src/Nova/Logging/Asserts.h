#pragma once
#include "Log.h"

#ifdef _MSC_VER
#define NOVA_DEBUGBREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
#define NOVA_DEBUGBREAK() __builtin_trap()
#endif

#ifdef DEBUG
#define NOVA_ASSERT(condition, ...) \
if (!condition)						\
{									\
	NOVA_CORE_ERROR(__VA_ARGS__);	\
	NOVA_DEBUGBREAK();				\
}
#else // Release
#define NOVA_ASSERT(condition, ...)
#endif
