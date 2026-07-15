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

	struct GizmoVertex
	{
		GizmoVertex(XMFLOAT3 position, XMFLOAT4 color)
		{
			Position = position;
			Color = color;
		}

		XMFLOAT3 Position;
		XMFLOAT4 Color;
	};

	template<typename T>
	concept VertexFormat =
		std::same_as<std::remove_cvref_t<T>, Vertex> ||
		std::same_as<std::remove_cvref_t<T>, GizmoVertex> ||
		std::same_as<std::remove_cvref_t<T>, SkyboxVertex>;
}
