#include "novapch.h"
#include "Mesh.h"
#include "Nova/Graphics/DX11.h"

Nova::Graphics::Mesh::Mesh()
{
	m_MeshData = std::make_unique<MeshData>();
}

Nova::Graphics::Mesh::Mesh(const MeshData& meshData, bool readWriteable) :
	m_VertexBuffer(meshData.Vertices),
	m_IndexBuffer(meshData.Indices),
	m_SubMeshes(meshData.SubMeshes)
{
	if (readWriteable) m_MeshData = std::make_unique<MeshData>(meshData);
}

void Nova::Graphics::Mesh::Bind() const
{
	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();
}

void Nova::Graphics::Mesh::DrawIndexed() const
{
	if (m_SubMeshes.empty())
	{
		DX11::GetContext()->DrawIndexed(m_IndexBuffer.Length(), 0, 0);
		return;
	}

	for (auto& subMesh : m_SubMeshes)
	{
		DX11::GetContext()->DrawIndexed(subMesh.IndexLength, subMesh.IndexOffset, subMesh.VertexOffset);
	}
}

bool Nova::Graphics::Mesh::ReadWriteable() const
{
	return m_MeshData != nullptr;
}

Nova::Graphics::MeshData* Nova::Graphics::Mesh::GetReadWriteData() const
{
	return m_MeshData.get();
}

void Nova::Graphics::Mesh::Apply(bool readWriteable)
{
	NOVA_ASSERT(ReadWriteable(), "Mesh isn't ReadWriteable!");

	m_SubMeshes = m_MeshData->SubMeshes;

	// TODO
}
