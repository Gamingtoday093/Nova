#pragma once
#include "Nova/Logging/Asserts.h"
#include <d3d11.h>

namespace Nova::Graphics::HRAsserts
{
	bool WriteErrors(HRESULT hr);
}

#ifdef DEBUG
#define NOVA_HRASSERT(hr, ...)						\
if (Nova::Graphics::HRAsserts::WriteErrors(hr))		\
{													\
	NOVA_CORE_ERROR(__VA_ARGS__);					\
	NOVA_DEBUGBREAK();								\
}
#else // Release
#define NOVA_HRASSERT(hr, ...) NOVA_ASSERT(SUCCEEDED(hr), __VA_ARGS__)
#endif
