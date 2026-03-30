#pragma once
#include "Nova/Logging/Asserts.h"
#include <d3d11.h>

#ifdef DEBUG
#define NOVA_HRASSERT(hr, ...) NOVA_ASSERT(SUCCEEDED(hr), __VA_ARGS__)
#else // Release
#define NOVA_HRASSERT(hr, ...) NOVA_ASSERT(SUCCEEDED(hr), __VA_ARGS__)
#endif