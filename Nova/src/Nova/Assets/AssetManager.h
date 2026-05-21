#pragma once
#include "Asset.h"

namespace Nova
{
	class ModelSourceAsset;
	class Texture2DAsset;
	class SkyboxAsset;
	class ShaderAsset;
}

namespace Nova
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		template<typename TAsset>
		static std::shared_ptr<TAsset> GetAsset(const std::filesystem::path& assetPath) { static_assert(false, "Unsupported SourceAsset"); }

		template<>
		static std::shared_ptr<ModelSourceAsset> GetAsset<ModelSourceAsset>(const std::filesystem::path& assetPath);

		template<>
		static std::shared_ptr<Texture2DAsset> GetAsset<Texture2DAsset>(const std::filesystem::path& assetPath);

		template<>
		static std::shared_ptr<SkyboxAsset> GetAsset<SkyboxAsset>(const std::filesystem::path& assetPath);

		template<>
		static std::shared_ptr<ShaderAsset> GetAsset<ShaderAsset>(const std::filesystem::path& assetPath);

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
		static std::shared_ptr<TAsset> LoadFromPath(const std::filesystem::path& assetPath);

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
