#include "pch.h"
#include "MeshAsset.h"
#include "Nova/Graphics/Resources/Mesh.h"

Nova::MeshAsset::MeshAsset(const AssetID& assetID, const std::string& name) : Asset(assetID, name) { }

std::shared_ptr<Nova::Graphics::Mesh> Nova::MeshAsset::GetMesh()
{
	if (m_Mesh) return m_Mesh;

	m_Mesh = std::make_shared<Graphics::Mesh>();
	return m_Mesh;
}
