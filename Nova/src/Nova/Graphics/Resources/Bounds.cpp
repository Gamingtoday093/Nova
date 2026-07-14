#include "novapch.h"
#include "Bounds.h"

Nova::Graphics::Bounds::Bounds(XMFLOAT3 center, XMFLOAT3 size)
{
	Center = center;
	Extents = { size.x * 0.5f, size.y * 0.5f, size.z * 0.5f };
}

XMFLOAT3 Nova::Graphics::Bounds::GetSize() const
{
	XMVECTOR extents = DirectX::XMLoadFloat3(&Extents);

	XMFLOAT3 size;
	DirectX::XMStoreFloat3(&size, extents * 2.f);
	return size;
}

XMFLOAT3 Nova::Graphics::Bounds::GetMin() const
{
	XMVECTOR center = DirectX::XMLoadFloat3(&Center);
	XMVECTOR extents = DirectX::XMLoadFloat3(&Extents);

	XMFLOAT3 min;
	DirectX::XMStoreFloat3(&min, center - extents);
	return min;
}

XMFLOAT3 Nova::Graphics::Bounds::GetMax() const
{
	XMVECTOR center = DirectX::XMLoadFloat3(&Center);
	XMVECTOR extents = DirectX::XMLoadFloat3(&Extents);

	XMFLOAT3 max;
	DirectX::XMStoreFloat3(&max, center + extents);
	return max;
}

bool XM_CALLCONV Nova::Graphics::Bounds::InsideBounds(XMVECTOR position) const
{
	XMVECTOR center = DirectX::XMLoadFloat3(&Center);
	XMVECTOR extents = DirectX::XMLoadFloat3(&Extents);

	position -= center;
	return DirectX::XMVector3InBounds(position, extents * 2.f);
}

bool XM_CALLCONV Nova::Graphics::Bounds::RayBounds(XMVECTOR origin, XMVECTOR direction, float* hitDistance) const
{
	XMVECTOR center = DirectX::XMLoadFloat3(&Center);
	XMVECTOR extents = DirectX::XMLoadFloat3(&Extents);
	XMVECTOR min = center - extents;
	XMVECTOR max = center + extents;

	XMVECTOR invDir = DirectX::XMVectorReciprocal(direction);
	XMVECTOR tMin = (min - origin) * invDir;
	XMVECTOR tMax = (max - origin) * invDir;
	XMVECTOR t1 = DirectX::XMVectorMin(tMin, tMax);
	XMVECTOR t2 = DirectX::XMVectorMax(tMin, tMax);
	XMVECTOR tNear = DirectX::XMVectorMax(DirectX::XMVectorMax(t1, DirectX::XMVectorSplatY(t1)), DirectX::XMVectorSplatZ(t1)); // Max of (t1.x, t1.y, t1.z)
	XMVECTOR tFar = DirectX::XMVectorMin(DirectX::XMVectorMin(t2, DirectX::XMVectorSplatY(t2)), DirectX::XMVectorSplatZ(t2)); // Min of (t2.x, t2.y, t2.z)
	if (hitDistance) *hitDistance = DirectX::XMVectorGetX(tNear);
	return DirectX::XMVectorGetX(tNear) <= DirectX::XMVectorGetX(tFar);
}

void XM_CALLCONV Nova::Graphics::Bounds::SetMinMax(XMVECTOR min, XMVECTOR max)
{
	XMVECTOR halfDistance = (max - min) * 0.5f;
	DirectX::XMStoreFloat3(&Center, min + halfDistance);
	DirectX::XMStoreFloat3(&Extents, DirectX::XMVectorAbs(halfDistance));
}

void XM_CALLCONV Nova::Graphics::Bounds::ExpandTo(XMVECTOR position)
{
	XMVECTOR center = DirectX::XMLoadFloat3(&Center);
	XMVECTOR extents = DirectX::XMLoadFloat3(&Extents);
	XMVECTOR min = center - extents;
	XMVECTOR max = center + extents;

	min = DirectX::XMVectorMin(min, position);
	max = DirectX::XMVectorMax(max, position);

	SetMinMax(min, max);
}

void Nova::Graphics::Bounds::MergeWith(const Bounds& other)
{
	XMVECTOR center = DirectX::XMLoadFloat3(&other.Center);
	XMVECTOR extents = DirectX::XMLoadFloat3(&other.Extents);
	XMVECTOR min = center - extents;
	XMVECTOR max = center + extents;

	ExpandTo(min);
	ExpandTo(max);
}
