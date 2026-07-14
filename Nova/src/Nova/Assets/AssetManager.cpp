#include "novapch.h"
#include "AssetManager.h"
#include "Importers/MeshImporter.h"
#include "Importers/TextureImporter.h"
#include "Importers/SkyboxImporter.h"
#include "Importers/ShaderImporter.h"


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

const std::filesystem::path& Nova::AssetManager::GetAssetsPath()
{
	static std::filesystem::path assetsPath = "Assets";
	return assetsPath;
}

const std::filesystem::path& Nova::AssetManager::GetAssetsFullPath()
{
	static std::filesystem::path assetsFullPath = std::filesystem::current_path() / GetAssetsPath();
	return assetsFullPath;
}

template<>
std::shared_ptr<Nova::ModelSourceAsset> Nova::AssetManager::GetAsset(const std::filesystem::path& assetPath)
{
	return LoadFromPath<ModelSourceAsset, Assets::MeshImporter>(assetPath);
}

template<>
std::shared_ptr<Nova::Texture2DAsset> Nova::AssetManager::GetAsset(const std::filesystem::path& assetPath)
{
	return LoadFromPath<Texture2DAsset, Assets::TextureImporter>(assetPath);
}

template<>
std::shared_ptr<Nova::SkyboxAsset> Nova::AssetManager::GetAsset(const std::filesystem::path& assetPath)
{
	return LoadFromPath<SkyboxAsset, Assets::SkyboxImporter>(assetPath);
}

template<>
std::shared_ptr<Nova::ShaderAsset> Nova::AssetManager::GetAsset(const std::filesystem::path& assetPath)
{
	return LoadFromPath<ShaderAsset, Assets::ShaderImporter>(assetPath);
}

template<Nova::SourceAssetType TAsset, class Importer>
std::shared_ptr<TAsset> Nova::AssetManager::LoadFromPath(const std::filesystem::path& assetPath)
{
	AssetManager& assetManager = AssetManager::Get();
	auto existingID = assetManager.m_PathToAssetID.find(assetPath);
	if (existingID != assetManager.m_PathToAssetID.end()) return GetAsset<TAsset>(existingID->second);

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

	assetManager.m_AssetRegistry.insert_or_assign(asset->GetAssetID(), asset);
	assetManager.m_NameToAssetID.insert_or_assign(asset->GetName(), asset->GetAssetID());
	assetManager.m_PathToAssetID.insert_or_assign(assetPath, asset->GetAssetID());
	return asset;
}

bool Nova::AssetManager::DestroyAsset(const AssetID& assetID)
{
	auto asset = GetAsset<Asset>(assetID);
	if (!asset) return false;

	asset->DisposeAsset();
	AssetManager& assetManager = AssetManager::Get();
	assetManager.m_AssetRegistry.erase(assetID);
	assetManager.m_NameToAssetID.erase(asset->GetName());
	if (asset->GetAssetType() == EAssetType::ModelSource) // TODO: Replace with some other way of figuring out if this is a SourceAsset
	{
		auto sourceAsset = std::static_pointer_cast<SourceAsset>(asset);
		assetManager.m_PathToAssetID.erase(sourceAsset->GetAssetPath());
	}
	return true;
}

Nova::AssetManager& Nova::AssetManager::Get()
{
	NOVA_ASSERT(m_Instance, "AssetManager hasn't been Initialized");
	return *m_Instance;
}
