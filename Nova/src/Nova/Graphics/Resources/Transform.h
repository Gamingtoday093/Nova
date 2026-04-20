#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace Nova::Graphics
{
	class Transform
	{
	public:
		XMFLOAT3 Position = { 0, 0, 0 };
		// Maybe also store as Quaternion?
		XMFLOAT3 Rotation = { 0, 0, 0 };
		XMFLOAT3 Scale = { 1, 1, 1 };

		XMMATRIX CalculateMatrix() const;
	};
}

