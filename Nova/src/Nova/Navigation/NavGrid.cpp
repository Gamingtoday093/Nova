#include "novapch.h"
#include "NavGrid.h"
#include "ValuedGridNode.h"

Nova::NavGrid::NavGrid(float nodeSize) : NodeSize(nodeSize) { }

Nova::NavGrid::NavGrid(uint16_t width, uint16_t height, float nodeSize) : NavGrid(nodeSize)
{
	Resize(width, height);
}

void Nova::NavGrid::Resize(uint16_t width, uint16_t height)
{
	m_Width = width;
	m_Height = height;

	m_Nodes.assign(size_t(m_Width) * size_t(m_Height), {});
}

void Nova::NavGrid::StampSquare(XMINT2 coord, int32_t radius, GridNode value)
{
	int32_t xMin = std::max(-radius, -coord.x);
	int32_t xMax = std::min(radius, (m_Width - 1) - coord.x);
	int32_t yMin = std::max(-radius, -coord.y);
	int32_t yMax = std::min(radius, (m_Height - 1) - coord.y);

	for (int32_t x = xMin; x <= xMax; x++)
		for (int32_t y = yMin; y <= yMax; y++)
			m_Nodes[size_t(((coord.x + x) * int32_t(m_Height)) + (coord.y + y))] = value;
}

void Nova::NavGrid::StampCircle(XMINT2 coord, int32_t radius, GridNode value)
{
	int32_t x = 0;
	int32_t y = -radius;
	int32_t p = -radius;

	// Fill Center
	StampSquare(coord, radius - (radius / 3), value);

	while (x < -y)
	{
		if (p > 0)
		{
			y++;
			p += 2 * (x + y) + 1;
		}
		else
		{
			p += 2 * x + 1;
		}

		int32_t lineLength = (radius / 3) - (radius + y);
		StampCircleLine({ coord.x + x, coord.y + y }, { coord.x + x, coord.y - y }, lineLength, value);
		StampCircleLine({ coord.x - x, coord.y + y }, { coord.x - x, coord.y - y }, lineLength, value);

		StampCircleLine({ coord.x + y, coord.y + x }, { coord.x - y, coord.y + x }, lineLength, value);
		StampCircleLine({ coord.x + y, coord.y - x }, { coord.x - y, coord.y - x }, lineLength, value);

		x++;
	}
}

void Nova::NavGrid::StampCircleLine(XMINT2 from, XMINT2 to, int32_t radius, GridNode value)
{
	int32_t stepX = 0, stepY = 0;
	if (from.x == to.x) stepY = from.y > to.y ? -1 : 1;
	else stepX = from.x > to.x ? -1 : 1;

	for (int32_t i = 0; i < radius; i++)
	{
		XMINT2 targetCoord = { from.x + (stepX * i), from.y + (stepY * i) };
		if (targetCoord.x >= 0 && targetCoord.x < m_Width &&
			targetCoord.y >= 0 && targetCoord.y < m_Height)
		{
			m_Nodes[size_t((targetCoord.x * m_Height) + targetCoord.y)] = value;
		}

		targetCoord = { to.x + (-stepX * i), to.y + (-stepY * i) };
		if (targetCoord.x >= 0 && targetCoord.x < m_Width && targetCoord.y >= 0 &&
			targetCoord.y < m_Height)
		{
			m_Nodes[size_t((targetCoord.x * m_Height) + targetCoord.y)] = value;
		}
	}
}

bool XM_CALLCONV Nova::NavGrid::Pathfind(XMVECTOR from, XMVECTOR to, std::vector<XMFLOAT3>& resultPath) const
{
	resultPath.clear();

	int32_t startNode = GetNearestIndex(from);
	int32_t endNode = GetNearestIndex(to);
	
	if (m_Nodes[startNode].IsOccupied() || m_Nodes[endNode].IsOccupied())
		return false;

	if (startNode == endNode)
	{
		resultPath.push_back(GetWorldPosition(startNode));
		return true;
	}

	int32_t lineOfSightNode = GetLineOfSight(startNode, endNode);
	if (lineOfSightNode == endNode)
	{
		resultPath.push_back(GetWorldPosition(startNode));
		resultPath.push_back(GetWorldPosition(endNode));
		return true;
	}

	std::unordered_map<int32_t, int32_t> childToParent;
	Navigation::OpenGridNodesHeap openNodes;
	std::unordered_set<int32_t> closedNodes;

	XMFLOAT3 tmp = GetWorldPosition(startNode);
	from = DirectX::XMLoadFloat3(&tmp);
	tmp = GetWorldPosition(endNode);
	to = DirectX::XMLoadFloat3(&tmp);
	float maxDistance = DirectX::XMVectorGetX(DirectX::XMVector3Length(to - from));

	childToParent[startNode] = startNode;
	XMVECTOR nodePosition = from;
	openNodes.emplace(startNode,
		0.f,
		DirectX::XMVectorGetX(DirectX::XMVector3Length(to - nodePosition)));

	if (lineOfSightNode != startNode)
	{
		childToParent[lineOfSightNode] = startNode;
		tmp = GetWorldPosition(lineOfSightNode);
		nodePosition = DirectX::XMLoadFloat3(&tmp);
		openNodes.emplace(lineOfSightNode,
			DirectX::XMVectorGetX(DirectX::XMVector3Length(nodePosition - from)),
			DirectX::XMVectorGetX(DirectX::XMVector3Length(to - nodePosition)));
	}

	while (!openNodes.empty())
	{
		Navigation::ValuedGridNode nextNode = openNodes.top();
		openNodes.pop();
		closedNodes.insert(nextNode.m_NodeIndex);

		if (nextNode.m_NodeIndex == endNode)
		{
		EndNodeFound:
			GetResultPath(startNode, endNode, childToParent, resultPath);
			break;
		}

		if (nextNode.m_NodeIndex != startNode && HasLineOfSight(nextNode.m_NodeIndex, endNode))
		{
			childToParent.insert_or_assign(endNode, nextNode.m_NodeIndex);
			goto EndNodeFound;
		}

		for (int32_t neighbourIndex : GetNeighbours(nextNode.m_NodeIndex))
		{
			if (m_Nodes[neighbourIndex].IsOccupied()) continue;

			if (neighbourIndex == endNode)
			{
				childToParent.insert_or_assign(endNode, nextNode.m_NodeIndex);
				goto EndNodeFound;
			}

			if (closedNodes.contains(neighbourIndex)) continue;
			else if (!openNodes.ContainsNodeIndex(neighbourIndex))
			{
				tmp = GetWorldPosition(neighbourIndex);
				nodePosition = DirectX::XMLoadFloat3(&tmp);
				float GCost = DirectX::XMVectorGetX(DirectX::XMVector3Length(nodePosition - from));
				float HCost = DirectX::XMVectorGetX(DirectX::XMVector3Length(to - nodePosition));
				if (GCost > maxDistance && HCost > maxDistance) return false; // Probably no Possible Path
				openNodes.emplace(neighbourIndex, GCost, HCost);
				childToParent.insert_or_assign(neighbourIndex, nextNode.m_NodeIndex);
			}
		}
	}

	return !resultPath.empty();
}

bool Nova::NavGrid::HasLineOfSight(int32_t startNodeIndex, int32_t endNodeIndex) const
{
	return GetLineOfSight(startNodeIndex, endNodeIndex) == endNodeIndex;
}

int32_t Nova::NavGrid::GetLineOfSight(int32_t startNodeIndex, int32_t endNodeIndex) const
{
	if (startNodeIndex == endNodeIndex && !m_Nodes[startNodeIndex].IsOccupied()) return endNodeIndex;

	XMINT2 startNode = GetCoord(startNodeIndex);
	XMINT2 endNode = GetCoord(endNodeIndex);

	int32_t deltaX = abs(startNode.x - endNode.x);
	int32_t deltaY = abs(startNode.y - endNode.y);

	int32_t stepX = startNode.x < endNode.x ? 1 : -1;
	int32_t stepY = startNode.y < endNode.y ? 1 : -1;

	int32_t xx, xy, yx, yy;
	if (deltaX > deltaY)
	{
		xx = stepX;
		xy = 0;
		yx = 0;
		yy = stepY;
	}
	else
	{
		std::swap(deltaX, deltaY);

		xx = 0;
		xy = stepY;
		yx = stepX;
		yy = 0;
	}

	int32_t D = (2 * deltaY) - deltaX;
	int32_t currentY = 0;

	size_t lastIndex = startNodeIndex;
	for (int32_t currentX = 0; currentX < deltaX + 1; currentX++)
	{
		int32_t indexWidth = startNode.x + currentX * xx + currentY * yx;
		int32_t indexHeight = startNode.y + currentX * xy + currentY * yy;
		size_t nodeIndex = size_t((indexWidth * int32_t(m_Height)) + indexHeight);
		if (m_Nodes[nodeIndex].IsOccupied()) return int32_t(lastIndex);
		lastIndex = nodeIndex;

		if (D >= 0)
		{
			currentY++;
			D -= 2 * deltaX;
		}
		D += 2 * deltaY;
	}

	return endNodeIndex;
}

void Nova::NavGrid::GetResultPath(int32_t startNodeIndex, int32_t endNodeIndex, const std::unordered_map<int32_t, int32_t>& childToParent, std::vector<XMFLOAT3>& resultPath) const
{
	int32_t currentNodeIndex = endNodeIndex;
	while (childToParent.at(currentNodeIndex) != currentNodeIndex)
	{
		resultPath.push_back(GetWorldPosition(currentNodeIndex));

		int32_t lineOfSightTarget = currentNodeIndex;
		currentNodeIndex = childToParent.at(currentNodeIndex);

		// -- Path Post-Processing --

		// Pathfind already performs a LineOfSight check to try and reach it's final node
		if (lineOfSightTarget == endNodeIndex) continue;

		int32_t LOSChild = lineOfSightTarget;
		while (childToParent.at(lineOfSightTarget) != lineOfSightTarget)
		{
			if (HasLineOfSight(currentNodeIndex, lineOfSightTarget))
			{
				LOSChild = lineOfSightTarget;
				lineOfSightTarget = childToParent.at(lineOfSightTarget);
				if (childToParent.at(lineOfSightTarget) == lineOfSightTarget)
				{
					currentNodeIndex = lineOfSightTarget;
					break;
				}
			}
			else if (HasLineOfSight(currentNodeIndex, startNodeIndex))
			{
				LOSChild = lineOfSightTarget;
				lineOfSightTarget = childToParent.at(lineOfSightTarget);
			}
			else
			{
				currentNodeIndex = LOSChild;
				break;
			}
		}
	}
	resultPath.push_back(GetWorldPosition(currentNodeIndex));

	std::reverse(resultPath.begin(), resultPath.end());
}

std::vector<int32_t> Nova::NavGrid::GetNeighbours(int32_t nodeIndex) const
{
	std::vector<int32_t> neighbours;
	neighbours.reserve(8);

	XMINT2 coord = GetCoord(nodeIndex);

	if (coord.y - 1 >= 0) neighbours.push_back(nodeIndex - 1);
	if (coord.y + 1 < m_Height) neighbours.push_back(nodeIndex + 1);

	if (coord.x - 1 >= 0)
	{
		if (coord.y - 1 >= 0) neighbours.push_back(nodeIndex - 1 - m_Height);
		neighbours.push_back(nodeIndex - m_Height);
		if (coord.y + 1 < m_Height) neighbours.push_back(nodeIndex + 1 - m_Height);
	}

	if (coord.x + 1 < m_Width)
	{
		if (coord.y - 1 >= 0) neighbours.push_back(nodeIndex - 1 + m_Height);
		neighbours.push_back(nodeIndex + m_Height);
		if (coord.y + 1 < m_Height) neighbours.push_back(nodeIndex + 1 + m_Height);
	}

	return neighbours;
}

XMINT2 Nova::NavGrid::GetCoord(int32_t nodeIndex) const
{
	return { (nodeIndex - (nodeIndex % m_Height)) / int32_t(m_Height), nodeIndex % m_Height };
}

int32_t Nova::NavGrid::GetIndex(XMINT2 coord) const
{
	return (coord.x * m_Height) + coord.y;
}

XMFLOAT3 Nova::NavGrid::GetWorldPosition(int32_t nodeIndex) const
{
	return GetWorldPosition(GetCoord(nodeIndex));
}

XMFLOAT3 Nova::NavGrid::GetWorldPosition(XMINT2 coord) const
{
	return { (coord.x * NodeSize) + (NodeSize * 0.5f), 0.f, (coord.y * NodeSize) + (NodeSize * 0.5f) };
}

int32_t XM_CALLCONV Nova::NavGrid::GetNearestIndex(XMVECTOR position) const
{
	return GetIndex(GetNearest(position));
}

XMINT2 XM_CALLCONV Nova::NavGrid::GetNearest(XMVECTOR position) const
{
	position /= NodeSize;
	XMINT2 coord = { int32_t(DirectX::XMVectorGetX(position)), int32_t(DirectX::XMVectorGetZ(position)) };

	if (coord.x < 0) coord.x = 0;
	else if (coord.x >= m_Width) coord.x = int32_t(m_Width) - 1;
	
	if (coord.y < 0) coord.y = 0;
	else if (coord.y >= m_Height) coord.y = int32_t(m_Height) - 1;

	return coord;
}

uint16_t Nova::NavGrid::GetWidth() const
{
	return m_Width;
}

uint16_t Nova::NavGrid::GetHeight() const
{
	return m_Height;
}

const std::vector<Nova::GridNode>& Nova::NavGrid::GetNodes() const
{
	return m_Nodes;
}
