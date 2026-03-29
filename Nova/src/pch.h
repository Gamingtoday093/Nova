#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <chrono>
#include "EnTT/entt.hpp"
#include <d3d11.h>
#include <wrl.h>

inline std::wstring ToWString(const std::string& stringLhs, const std::string& stringRhs)
{
	std::string combined(stringLhs + stringRhs);
	return std::wstring(combined.begin(), combined.end());
}
