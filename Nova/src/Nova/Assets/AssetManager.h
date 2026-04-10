#pragma once
#include "AssetFormats/MeshSourceAsset.h"

namespace Nova
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		template<SourceAssetType TAsset>
		static std::shared_ptr<TAsset> GetAsset(const std::filesystem::path& assetPath) { static_assert(false); }

		template<>
		static std::shared_ptr<MeshSourceAsset> GetAsset<MeshSourceAsset>(const std::filesystem::path& assetPath);

		template<AssetType TAsset>
		static std::shared_ptr<TAsset> GetAsset(const AssetID& assetID);

		template<AssetType TAsset, typename... Args>
		static std::shared_ptr<TAsset> CreateAsset(const AssetID& assetID, const std::string& name, Args&&... args);

		static bool DestroyAsset(const AssetID& assetID);

	private:
		using AssetRegistry = std::unordered_map<AssetID, std::shared_ptr<Asset>>;
		template<typename T>
		using ToAssetID = std::unordered_map<T, AssetID>;

		inline static AssetManager& Get()
		{
			NOVA_ASSERT(m_Instance, "AssetManager hasn't been Initialized");
			return *m_Instance;
		}

		template<SourceAssetType TAsset, class Importer>
		static std::shared_ptr<TAsset> LoadFromFile(const std::filesystem::path& assetPath);

		AssetRegistry m_AssetRegistry;
		ToAssetID<std::string> m_NameToAssetID;
		ToAssetID<std::filesystem::path> m_PathToAssetID;

		static AssetManager* m_Instance;
	};

	template<AssetType TAsset>
	std::shared_ptr<TAsset> Nova::AssetManager::GetAsset(const AssetID& assetID)
	{
		auto asset = Get().m_AssetRegistry.find(assetID);
		if (asset == Get().m_AssetRegistry.end())
		{
			NOVA_CORE_WARN("Failed to find Get {0} from ID", TAsset::GetAssetName_s());
			return nullptr;
		}

		if (TAsset::GetAssetType_s() != EAssetType::None && asset->second->GetAssetType() != TAsset::GetAssetType_s())
		{
			NOVA_CORE_WARN("Asset with ID does is not a {0}", TAsset::GetAssetName_s());
			return nullptr;
		}

		return std::static_pointer_cast<TAsset>(asset->second);
	}

	template<AssetType TAsset, typename... Args>
	std::shared_ptr<TAsset> AssetManager::CreateAsset(const AssetID& assetID, const std::string& name, Args&&... args)
	{
		std::shared_ptr<TAsset> asset = std::make_shared<TAsset>(assetID, name, std::forward<Args>(args)...);

		Get().m_AssetRegistry.insert_or_assign(asset->GetAssetID(), asset);
		Get().m_NameToAssetID.insert_or_assign(asset->GetName(), asset->GetAssetID());
		return asset;
	}
}
