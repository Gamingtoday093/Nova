#include "pch.h"
#include "IndexBuffer.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

void Nova::Graphics::IndexBuffer::Create(const std::vector<uint16_t>& indices)
{
	m_IndexBufferLength = uint32_t(indices.size());

	D3D11_BUFFER_DESC indexBufferDesc
	{
		.ByteWidth = sizeof(uint16_t) * m_IndexBufferLength,
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_INDEX_BUFFER
	};

	D3D11_SUBRESOURCE_DATA indexData
	{
		.pSysMem = std::data(indices)
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer), "Create Index Buffer");
}

void Nova::Graphics::IndexBuffer::Bind() const
{
	NOVA_ASSERT(m_IndexBuffer.Get(), "Index Buffer not Created");
	DX11::GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

UINT Nova::Graphics::IndexBuffer::Length() const
{
	return m_IndexBufferLength;
}
