#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace Nova::Graphics
{
	class Transform
	{
	public:
		XMFLOAT3 Position = { 0, 0, 0 };
		XMFLOAT4 Rotation = { 0, 0, 0, 1 };
		XMFLOAT3 Scale = { 1, 1, 1 };

		XMFLOAT3 GetEulerAngles() const;
		void SetEulerAngles(const XMFLOAT3& rotation);

		XMMATRIX CalculateMatrix() const;
	};
}

