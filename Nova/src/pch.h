#pragma once

// std
#include <string>
#include <chrono>
#include <memory>
#include <vector>
#include <unordered_map>

// dx11
#include <wrl.h>
#include <d3d11.h>

// src
#include "Nova/Logging/Log.h"

// vendor
#include <EnTT/entt.hpp>

inline std::wstring ToWString(const std::string& stringLhs, const std::string& stringRhs)
{
	std::string combined(stringLhs + stringRhs);
	return std::wstring(combined.begin(), combined.end());
}
