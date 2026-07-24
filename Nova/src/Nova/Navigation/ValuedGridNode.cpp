#include "novapch.h"
#include "ValuedGridNode.h"

Nova::Navigation::ValuedGridNode::ValuedGridNode(int32_t nodeIndex, float GCost, float HCost)
{
	m_NodeIndex = nodeIndex;
	m_FCost = GCost + (GOAL_BIAS * HCost);
}

bool Nova::Navigation::ValuedGridNodeComparer::operator()(const ValuedGridNode& _Left, const ValuedGridNode& _Right) const
{
	return _Left.m_FCost > _Right.m_FCost;
}

bool Nova::Navigation::OpenGridNodesHeap::ContainsNodeIndex(int32_t nodeIndex) const
{
	for (ValuedGridNode node : c)
		if (node.m_NodeIndex == nodeIndex) return true;

	return false;
}
