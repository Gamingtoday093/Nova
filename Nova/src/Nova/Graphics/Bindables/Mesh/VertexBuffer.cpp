#include "pch.h"
#include "VertexBuffer.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

void Nova::Graphics::VertexBuffer::Create(const std::vector<Nova::Graphics::Vertex>& vertices)
{
	m_VertexElementSize = sizeof(Vertex);

	D3D11_BUFFER_DESC vertexBufferDesc
	{
		.ByteWidth = m_VertexElementSize * uint32_t(vertices.size()),
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER
	};

	D3D11_SUBRESOURCE_DATA vertexData
	{
		.pSysMem = std::data(vertices)
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer), "Create Vertex Buffer");
}

void Nova::Graphics::VertexBuffer::Bind() const
{
	NOVA_ASSERT(m_VertexBuffer.Get(), "Vertex Buffer not Created");
	static constexpr uint32_t offsets = 0u;
	DX11::GetContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VertexElementSize, &offsets);
}
