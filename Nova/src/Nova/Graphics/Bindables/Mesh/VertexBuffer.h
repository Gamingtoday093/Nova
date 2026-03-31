#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "Nova/Graphics/RenderData/VertexFormats.h"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class VertexBuffer : public IBindable
	{
	public:
		void Create(const std::vector<Vertex>& vertices);

		void Bind() const override;

	private:
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		uint32_t m_VertexBufferLength;
	};
}
