#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace Nova
{
	class Camera
	{
	public:
		virtual ~Camera() = default;

		virtual XMMATRIX GetViewMatrix() const = 0;

		/// <summary>
		/// Screen Position is (0, 0) Top Left and (1, 1) Bottom Right
		/// </summary>
		XMVECTOR XM_CALLCONV ScreenToWorld(XMVECTOR position) const;
		XMVECTOR XM_CALLCONV WorldToScreen(XMVECTOR position) const;

		float FovAngle = 1.f;
		float NearClipPlane = 0.1f;
		float FarClipPlane = 1000.f;
	};
}

