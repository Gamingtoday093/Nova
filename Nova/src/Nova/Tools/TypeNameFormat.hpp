#pragma once

namespace Nova::TypeNameFormat
{
	template<size_t N>
	struct FixedString
	{
		std::array<char, N> Data{};

		constexpr const char* CStr() const { return Data.data(); }
	};

	constexpr bool IsUpper(char ch)
	{
		return ch >= 'A' && ch <= 'Z';
	}

	template<size_t N, size_t S>
	constexpr auto FormatTypeName(const char (&typeName)[N], const char (&suffix)[S])
	{
		std::string_view typeNameView = std::string_view(typeName);
		std::string_view suffixView = std::string_view(suffix);

		constexpr size_t TERMINATOR = 1;
		constexpr size_t SPACE = 1;

		size_t nextBufferIndex = 0;
		FixedString<((N - TERMINATOR) * 2) + SPACE + (S - TERMINATOR) + TERMINATOR> formattedTypeName;

		formattedTypeName.Data[nextBufferIndex++] = typeName[0];
		for (size_t i = 1; i < typeNameView.size(); i++)
		{
			char ch = typeNameView[i];

			if (IsUpper(ch) && !IsUpper(typeNameView[i - 1]))
				formattedTypeName.Data[nextBufferIndex++] = ' ';

			formattedTypeName.Data[nextBufferIndex++] = ch;
		}

		if (!typeNameView.ends_with(suffix))
		{
			formattedTypeName.Data.at(nextBufferIndex++) = ' ';
			for (size_t i = 0; i < suffixView.size(); i++)
				formattedTypeName.Data.at(nextBufferIndex++) = suffixView[i];
		}

		formattedTypeName.Data.at(nextBufferIndex++) = '\0';
		return formattedTypeName;
	}
}
