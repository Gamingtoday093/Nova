#pragma once
#include "GridNode.h"

using namespace DirectX;

namespace Nova
{
	class NavGrid
	{
	public:
		NavGrid(float nodeSize = 1.f);
		NavGrid(uint16_t width, uint16_t height, float nodeSize = 1.f);

		void Resize(uint16_t width, uint16_t height);
		void StampSquare(XMINT2 coord, int32_t radius, GridNode value);
		void StampCircle(XMINT2 coord, int32_t radius, GridNode value);

		bool XM_CALLCONV Pathfind(XMVECTOR from, XMVECTOR to, std::vector<XMFLOAT3>& resultPath) const;

		XMINT2 GetCoord(int32_t nodeIndex) const;
		int32_t GetIndex(XMINT2 coord) const;
		XMFLOAT3 GetWorldPosition(int32_t nodeIndex) const;
		XMFLOAT3 GetWorldPosition(XMINT2 coord) const;
		int32_t XM_CALLCONV GetNearestIndex(XMVECTOR position) const;
		XMINT2 XM_CALLCONV GetNearest(XMVECTOR position) const;

		uint16_t GetWidth() const;
		uint16_t GetHeight() const;
		float NodeSize;
		const std::vector<GridNode>& GetNodes() const;

	private:
		void StampCircleLine(XMINT2 from, XMINT2 to, int32_t radius, GridNode value);

		bool HasLineOfSight(int32_t startNodeIndex, int32_t endNodeIndex, bool allowCost = false) const;
		int32_t GetLineOfSight(int32_t startNodeIndex, int32_t endNodeIndex, bool allowCost = false) const;
		void GetResultPath(int32_t startNodeIndex, int32_t endNodeIndex, const std::unordered_map<int32_t, int32_t>& childToParent, std::vector<XMFLOAT3>& resultPath) const;
		std::vector<int32_t> GetNeighbours(int32_t nodeIndex) const;

		uint16_t m_Width = 0, m_Height = 0;
		std::vector<GridNode> m_Nodes;
	};
}
