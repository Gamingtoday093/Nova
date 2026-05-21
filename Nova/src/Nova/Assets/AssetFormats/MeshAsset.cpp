#include "novapch.h"
#include "MeshAsset.h"
#include "ModelSourceAsset.h"
#include "Nova/Graphics/Resources/Mesh.h"

bool Nova::MeshOptionalSettings::operator==(const MeshOptionalSettings& other) const
{
	return memcmp(this, &other, sizeof(MeshOptionalSettings)) == 0;
}

Nova::MeshAsset::MeshAsset(const AssetID& assetID, const std::string& name, std::weak_ptr<ModelSourceAsset> meshSource) : Asset(assetID, name), m_MeshSource(std::move(meshSource)) { }

const Nova::MeshOptionalSettings& Nova::MeshAsset::GetSettings() const
{
	return m_Settings;
}

void Nova::MeshAsset::SetSettings(const MeshOptionalSettings& settings)
{
	bool needsReload = m_Settings != settings;
	m_Settings = settings;
	if (needsReload) ReloadFromSource();
}

Nova::AssetRef<Nova::Graphics::Mesh> Nova::MeshAsset::GetMesh()
{
	if (m_Mesh) return m_Mesh;

	ReloadFromSource();
	NOVA_CORE_WARN("MeshAsset ({0}) has been unloaded and is forced to reload from source!", GetAssetID().ToString());
	NOVA_ASSERT(m_Mesh, "Invalid MeshAsset");
	return m_Mesh;
}

void Nova::MeshAsset::ReloadFromSource()
{
	if (auto meshSource = m_MeshSource.lock())
		meshSource->ReloadFromSource(); // Populates m_Mesh
}

void Nova::MeshAsset::Invalidate()
{
	m_Mesh = nullptr;
}

void Nova::MeshAsset::DisposeAsset()
{
	Invalidate();
	m_MeshSource.reset();
}
