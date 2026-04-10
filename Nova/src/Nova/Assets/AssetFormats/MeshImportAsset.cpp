#include "novapch.h"
#include "MeshImportAsset.h"
#include "MeshSourceAsset.h"

bool Nova::MeshOptionalSettings::operator==(const MeshOptionalSettings& other) const
{
	return memcmp(this, &other, sizeof(MeshOptionalSettings)) == 0;
}

Nova::MeshImportAsset::MeshImportAsset(const AssetID& assetID, const std::string& name, std::weak_ptr<MeshSourceAsset> meshSource) : MeshAsset(assetID, name), m_MeshSource(std::move(meshSource)) { }

const Nova::MeshOptionalSettings& Nova::MeshImportAsset::GetSettings() const
{
	return m_Settings;
}

void Nova::MeshImportAsset::SetSettings(const MeshOptionalSettings& settings)
{
	bool needsReload = m_Settings != settings;
	m_Settings = settings;
	if (needsReload) ReloadFromSource();
}

std::shared_ptr<Nova::Graphics::Mesh> Nova::MeshImportAsset::GetMesh()
{
	if (m_Mesh) return m_Mesh;

	ReloadFromSource();
	NOVA_CORE_WARN("MeshImportAsset ({0}) has been unloaded and is forced to reload from source!", GetAssetID().ToString());
	return m_Mesh;
}

void Nova::MeshImportAsset::ReloadFromSource()
{
	if (auto meshSource = m_MeshSource.lock())
		meshSource->ReloadFromSource(); // Populates m_Mesh
}

void Nova::MeshImportAsset::Invalidate()
{
	m_Mesh = nullptr;
}

void Nova::MeshImportAsset::DisposeAsset()
{
	Invalidate();
	m_MeshSource.reset();
}
