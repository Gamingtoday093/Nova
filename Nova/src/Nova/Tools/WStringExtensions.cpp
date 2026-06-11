#include "novapch.h"
#include "WStringExtensions.h"

std::wstring Nova::ToWString(const std::string& stringLhs, const std::string& stringRhs)
{
	std::string combined(stringLhs + stringRhs);
	return std::wstring(combined.begin(), combined.end());
}

std::wstring Nova::ToWString(const std::string& string)
{
	return std::wstring(string.begin(), string.end());
}
