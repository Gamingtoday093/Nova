#include "novapch.h"
#include "AssetManager.h"
#include "Importers/MeshImporter.h"
#include "Importers/SkyboxImporter.h"


Nova::AssetManager* Nova::AssetManager::m_Instance;

Nova::AssetManager::AssetManager()
{
	NOVA_ASSERT(!m_Instance, "Creating multiple AssetManagers is not allowed");
	m_Instance = this;
}

Nova::AssetManager::~AssetManager()
{
	m_Instance = nullptr;
}

template<>
std::shared_ptr<Nova::MeshSourceAsset> Nova::AssetManager::GetAsset(const std::filesystem::path& assetPath)
{
	return LoadFromPath<MeshSourceAsset, Assets::MeshImporter>(assetPath);
}

template<>
std::shared_ptr<Nova::SkyboxAsset> Nova::AssetManager::GetAsset(const std::filesystem::path& assetPath)
{
	return LoadFromPath<SkyboxAsset, Assets::SkyboxImporter>(assetPath);
}

template<Nova::SourceAssetType TAsset, class Importer>
std::shared_ptr<TAsset> Nova::AssetManager::LoadFromPath(const std::filesystem::path& assetPath)
{
	auto existingID = Get().m_PathToAssetID.find(assetPath);
	if (existingID != Get().m_PathToAssetID.end()) return GetAsset<TAsset>(existingID->second);

	if (!std::filesystem::exists(assetPath))
	{
		NOVA_CORE_WARN("Unable to find {0} from {1}", TAsset::GetAssetName_s(), assetPath.string());
		return nullptr;
	}

	if (!Importer::Supported(assetPath))
	{
		NOVA_CORE_WARN("Importer does not allow {0} from {1}", TAsset::GetAssetName_s(), assetPath.has_extension() ? assetPath.extension().string() : assetPath.string());
		return nullptr;
	}

	std::shared_ptr<TAsset> asset = Importer::LoadFromPath(assetPath);
	if (!asset)
	{
		NOVA_CORE_ERROR("Importer failed to load {0} from {1}", TAsset::GetAssetName_s(), assetPath.string());
		return nullptr;
	}

	Get().m_AssetRegistry.insert_or_assign(asset->GetAssetID(), asset);
	Get().m_NameToAssetID.insert_or_assign(asset->GetName(), asset->GetAssetID());
	Get().m_PathToAssetID.insert_or_assign(assetPath, asset->GetAssetID());
	return asset;
}

bool Nova::AssetManager::DestroyAsset(const AssetID& assetID)
{
	auto asset = GetAsset<Asset>(assetID);
	if (!asset) return false;

	asset->DisposeAsset();
	Get().m_AssetRegistry.erase(assetID);
	Get().m_NameToAssetID.erase(asset->GetName());
	if (asset->GetAssetType() == EAssetType::MeshSource) // TODO: Replace with some other way of figuring out if this is a SourceAsset
	{
		auto sourceAsset = std::static_pointer_cast<SourceAsset>(asset);
		Get().m_PathToAssetID.erase(sourceAsset->GetAssetPath());
	}
	return true;
}
