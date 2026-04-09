#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "Nova/Graphics/RenderData/VertexFormats.h"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class VertexBuffer : public IBindable
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(const void* vertexData, uint32_t vertexDataSize, uint32_t vertexElementSize);
		VertexBuffer(const std::vector<Vertex>& vertices);
		void Create(const std::vector<Vertex>& vertices);

		void Bind() const override;

	private:
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		uint32_t m_VertexElementSize = 0;
	};
}
