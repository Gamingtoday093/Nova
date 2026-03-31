#pragma once
#include <DirectXMath.h>

namespace Nova::Graphics
{
	__declspec(align(16))
	struct TransformBuffer
	{
		DirectX::XMMATRIX ProjectionViewMatrix;
		DirectX::XMMATRIX ModelMatrix;
	};
}
