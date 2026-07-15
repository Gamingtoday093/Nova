#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace Nova::Graphics
{
	struct Line
	{
		Line() = default;
		Line(XMFLOAT3 startPosition, XMFLOAT3 endPosition, XMFLOAT4 color)
		{
			StartPosition = startPosition;
			EndPosition = endPosition;
			Color = color;
		}

		XMFLOAT3 StartPosition = {};
		XMFLOAT3 EndPosition = {};
		XMFLOAT4 Color = { 0, 1, 0, 1 };
	};
}
