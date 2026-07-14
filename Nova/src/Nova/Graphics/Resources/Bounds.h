#pragma once

using namespace DirectX;

namespace Nova::Graphics
{
	struct Bounds
	{
		XMFLOAT3 Center = {};
		XMFLOAT3 Extents = {};

		Bounds() = default;
		Bounds(XMFLOAT3 center, XMFLOAT3 size);

		XMFLOAT3 GetSize() const;
		XMFLOAT3 GetMin() const;
		XMFLOAT3 GetMax() const;

		bool XM_CALLCONV InsideBounds(XMVECTOR position) const;
		bool XM_CALLCONV RayBounds(XMVECTOR position, XMVECTOR direction, float* hitDistance = nullptr) const;

		void XM_CALLCONV SetMinMax(XMVECTOR min, XMVECTOR max);
		void XM_CALLCONV ExpandTo(XMVECTOR position);
		void MergeWith(const Bounds& other);
	};
}
