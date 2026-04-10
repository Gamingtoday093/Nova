#pragma once
#include "Nova/Logging/Asserts.h"
#include <d3d11.h>

namespace Nova::Graphics::HRAsserts
{
	bool WriteErrors(HRESULT hr);
}

#ifdef DEBUG
#define NOVA_HRASSERT(hr, ...) NOVA_ASSERT(!Nova::Graphics::HRAsserts::WriteErrors(hr), __VA_ARGS__);
#else // Release
#define NOVA_HRASSERT(hr, ...) NOVA_ASSERT(SUCCEEDED(hr), __VA_ARGS__)
#endif
