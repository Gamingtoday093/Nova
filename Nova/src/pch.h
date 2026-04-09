#pragma once

// std
#include <string>
#include <filesystem>
#include <chrono>
#include <memory>
#include <vector>
#include <unordered_map>

// dx11
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

// src
#include "Nova/Logging/Log.h"
#include "Nova/Logging/Asserts.h"

// vendor
#include <EnTT/entt.hpp>

inline std::wstring ToWString(const std::string& stringLhs, const std::string& stringRhs)
{
	std::string combined(stringLhs + stringRhs);
	return std::wstring(combined.begin(), combined.end());
}

inline std::wstring ToWString(const std::string& string)
{
	return std::wstring(string.begin(), string.end());
}
