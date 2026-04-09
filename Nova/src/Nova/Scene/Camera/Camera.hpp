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

		float FovAngle = 1.f;
		float NearClipPlane = 0.1f;
		float FarClipPlane = 1000.f;
	};
}

