#pragma once
#include "Nova/Graphics/Bindables/Mesh/VertexBuffer.h"
#include "Nova/Graphics/Bindables/Mesh/IndexBuffer.h"

namespace Nova::Graphics
{
	class SkyboxCube
	{
	public:
		SkyboxCube();

		void Bind() const;
		void DrawIndexed() const;

	private:
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
	};
}

