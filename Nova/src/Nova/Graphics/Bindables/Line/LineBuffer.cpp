#include "novapch.h"
#include "LineBuffer.h"
#include "Nova/Graphics/RenderData/VertexFormats.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include "Nova/Graphics/DX11.h"

Nova::Graphics::LineBuffer::LineBuffer(uint32_t maxLines)
{
	Resize(maxLines);
}

void Nova::Graphics::LineBuffer::Resize(uint32_t newMaxLines)
{
	NOVA_ASSERT(newMaxLines <= MAX_LINES, "LineBuffer doesn't Support more than {} Lines", MAX_LINES);

	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;
	m_MaxLines = newMaxLines;

	Lines.reserve(m_MaxLines);

	D3D11_BUFFER_DESC vertexBufferDesc
	{
		.ByteWidth = (sizeof(GizmoVertex) * 2) * m_MaxLines, // 2 Vertices per Line
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateBuffer(&vertexBufferDesc, nullptr, &m_VertexBuffer), "Create Line Vertex Buffer");

	D3D11_BUFFER_DESC indexBufferDesc
	{
		.ByteWidth = (sizeof(uint16_t) * 2) * m_MaxLines, // 2 Indices per Line
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateBuffer(&indexBufferDesc, nullptr, &m_IndexBuffer), "Create Line Index Buffer");
}

void Nova::Graphics::LineBuffer::ApplyChanges()
{
	NOVA_ASSERT(Lines.size() <= m_MaxLines, "LineBuffer can't Apply {0} Lines as the Max Lines is {1}", Lines.size(), m_MaxLines);

	D3D11_MAPPED_SUBRESOURCE vertexBufferData = {}, indexBufferData = {};
	NOVA_HRASSERT(DX11::GetContext()->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexBufferData), "Map Vertex Buffer");
	NOVA_HRASSERT(DX11::GetContext()->Map(m_IndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &indexBufferData), "Map Index Buffer");

	GizmoVertex* vertexBuffer = static_cast<GizmoVertex*>(vertexBufferData.pData);
	uint16_t* indexBuffer = static_cast<uint16_t*>(indexBufferData.pData);
	for (size_t i = 0; i < Lines.size(); i++)
	{
		const Line& line = Lines[i];

		vertexBuffer[i * 2 + 0] = { line.StartPosition, line.Color };
		vertexBuffer[i * 2 + 1] = { line.EndPosition, line.Color };

		indexBuffer[i * 2 + 0] = uint16_t(i * 2 + 0);
		indexBuffer[i * 2 + 1] = uint16_t(i * 2 + 1);
	}

	DX11::GetContext()->Unmap(m_VertexBuffer.Get(), 0);
	DX11::GetContext()->Unmap(m_IndexBuffer.Get(), 0);
}

void Nova::Graphics::LineBuffer::Bind() const
{
	static constexpr uint32_t stride = sizeof(GizmoVertex);
	static constexpr uint32_t offsets = 0u;

	DX11::GetContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offsets);
	DX11::GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

uint32_t Nova::Graphics::LineBuffer::GetMaxLines() const
{
	return m_MaxLines;
}
