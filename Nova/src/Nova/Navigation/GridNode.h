#pragma once
#include <stdint.h>

namespace Nova
{
	struct GridNode
	{
		GridNode() = default;
		GridNode(uint8_t value) : Value(value) {};

		bool IsOccupied() const { return Value > 0; }

		uint8_t Value = 0;
	};
}
