#include "pch.h"
#include "Mesh.h"

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

uint32_t Nova::Graphics::Mesh::GetIndexLength() const
{
	return m_IndexBuffer.Length();
}

std::span<const Nova::Graphics::SubMesh> Nova::Graphics::Mesh::GetSubMeshes() const
{
	return m_SubMeshes;
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
