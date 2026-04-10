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
		template<VertexFormat TVertex>
		VertexBuffer(const std::vector<TVertex>& vertices) { Create(vertices); }

		template<VertexFormat TVertex>
		void Create(const std::vector<TVertex>& vertices);
		void Create(const void* vertexData, uint32_t vertexDataSize, uint32_t vertexElementSize);

		void Bind() const override;

	private:
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		uint32_t m_VertexElementSize = 0;
	};

	template<VertexFormat TVertex>
	void VertexBuffer::Create(const std::vector<TVertex>& vertices)
	{
		Create(std::data(vertices), sizeof(TVertex) * uint32_t(vertices.size()), sizeof(TVertex));
	}
}
