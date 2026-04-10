#include "pch.h"
#include "VertexBuffer.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

Nova::Graphics::VertexBuffer::VertexBuffer(const void* vertexData, uint32_t vertexDataSize, uint32_t vertexElementSize)
{
	Create(vertexData, vertexDataSize, vertexElementSize);
}

void Nova::Graphics::VertexBuffer::Create(const void* vertexData, uint32_t vertexDataSize, uint32_t vertexElementSize)
{
	m_VertexElementSize = vertexElementSize;

	D3D11_BUFFER_DESC vertexBufferDesc
	{
		.ByteWidth = vertexDataSize,
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER
	};

	D3D11_SUBRESOURCE_DATA vertexInitialData
	{
		.pSysMem = vertexData
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexInitialData, &m_VertexBuffer), "Create Vertex Buffer");
}

void Nova::Graphics::VertexBuffer::Bind() const
{
	NOVA_ASSERT(m_VertexBuffer.Get(), "Vertex Buffer not Created");
	static constexpr uint32_t offsets = 0u;
	DX11::GetContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VertexElementSize, &offsets);
}
