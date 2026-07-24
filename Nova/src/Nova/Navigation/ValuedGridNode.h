#pragma once
#include <queue>

using namespace DirectX;

namespace Nova::Navigation
{
	struct ValuedGridNode
	{
		ValuedGridNode(int32_t nodeIndex, float GCost, float HCost);

		static constexpr float GOAL_BIAS = 2.f;

		int32_t m_NodeIndex;
		float m_FCost;
	};

	struct ValuedGridNodeComparer
	{
		bool operator()(const ValuedGridNode& _Left, const ValuedGridNode& _Right) const;
	};

	class OpenGridNodesHeap : public std::priority_queue<ValuedGridNode, std::vector<ValuedGridNode>, ValuedGridNodeComparer>
	{
	public:
		bool ContainsNodeIndex(int32_t nodeIndex) const;
	};
}
