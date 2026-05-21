#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace Nova::Graphics
{
	struct Vertex
	{
		Vertex(float x, float y, float z, float u, float v)
		{
			Position = { x, y, z };
			TexCoord = { u, v };
		}

		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
	};

	struct SkyboxVertex
	{
		SkyboxVertex(float x, float y, float z)
		{
			Position = { x, y, z };
		}

		XMFLOAT3 Position;
	};

	template<typename T>
	concept VertexFormat =
		std::same_as<std::remove_cvref_t<T>, Vertex> ||
		std::same_as<std::remove_cvref_t<T>, SkyboxVertex>;
}
