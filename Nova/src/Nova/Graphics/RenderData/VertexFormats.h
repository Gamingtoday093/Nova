#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace Nova::Graphics
{
	struct Vertex
	{
		Vertex(float x, float y, float z)
		{
			Position = { x, y, z };
			Color = { 0, 1, 1 };
		}

		XMFLOAT3 Position;
		XMFLOAT3 Color;
	};

	template<typename T>
	concept VertexFormat = std::same_as<std::remove_cvref_t<T>, Vertex>;
}
