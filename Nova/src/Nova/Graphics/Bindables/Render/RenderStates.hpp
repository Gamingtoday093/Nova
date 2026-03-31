#pragma once
#include <cstdint>

namespace Nova::Graphics
{
	enum class EBlendState : uint8_t
	{
		Opaque,
		Alpha,
		Additive,
		Multiply,
		Premultiplied,
		Count
	};

	enum class EDepthMode : uint8_t
	{
		ReadWrite,
		ReadOnly,
		Disabled,
		Count
	};

	enum class ECullMode : uint8_t
	{
		Back,
		Front,
		None,
		Count
	};
}
