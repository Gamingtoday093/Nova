#pragma once
#include <stdint.h>

namespace Nova
{
	struct GridNode
	{
		static constexpr uint8_t OCCUPIED = 0xff;

		GridNode() = default;
		GridNode(uint8_t value) : Value(value) {};

		bool IsOccupied() const { return Value == OCCUPIED; }
		bool HasCost() const { return Value > 0; }

		bool operator ==(GridNode other) const { return Value == other.Value; }

		uint8_t Value = 0;
	};
}
