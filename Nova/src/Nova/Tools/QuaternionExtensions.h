#pragma once
#include <DirectXMath.h>

namespace DirectX
{
	XMFLOAT3 XM_CALLCONV XMQuaternionToEulerAngles(XMVECTOR rotation);
	XMVECTOR XM_CALLCONV XMQuaternionFromToRotation(XMVECTOR from, XMVECTOR to);
	XMVECTOR XM_CALLCONV XMQuaternionLookRotation(XMVECTOR forward);
	XMVECTOR XM_CALLCONV XMQuaternionLookRotation(XMVECTOR forward, XMVECTOR up);
}