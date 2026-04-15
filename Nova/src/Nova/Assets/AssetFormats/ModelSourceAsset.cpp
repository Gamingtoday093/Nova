#include "novapch.h"
#include "ModelSourceAsset.h"
#include "MeshImportAsset.h"
#include "Nova/Assets/AssetManager.h"
#include "Nova/Assets/Importers/MeshImporter.h"

bool Nova::MeshImportSettings::operator==(const MeshImportSettings& other) const
{
	return memcmp(this, &other, sizeof(MeshImportSettings)) == 0;
}

Nova::ModelSourceAsset::ModelSourceAsset(const AssetID& assetID, const std::filesystem::path& assetPath) : SourceAsset(assetID, assetPath) { }

const Nova::MeshImportSettings& Nova::ModelSourceAsset::GetSettings() const
{
	return m_Settings;
}

void Nova::ModelSourceAsset::SetSettings(const MeshImportSettings& settings)
{
	bool needsReload = m_Settings != settings;
	m_Settings = settings;
	if (needsReload) ReloadFromSource();
}

void Nova::ModelSourceAsset::ReloadFromSource()
{
	Assets::MeshImporter::ReloadFromPath(AssetManager::GetAsset<ModelSourceAsset>(GetAssetID()));
}

void Nova::ModelSourceAsset::UnloadUnused(bool loadedAssets)
{
	for (auto& meshImport : m_MeshAssets)
	{
		if (loadedAssets || meshImport->GetMesh().use_count() <= 2)
			meshImport->Invalidate();
	}
}

const std::vector<std::shared_ptr<Nova::MeshImportAsset>>& Nova::ModelSourceAsset::GetMeshAssets() const
{
	return m_MeshAssets;
}
