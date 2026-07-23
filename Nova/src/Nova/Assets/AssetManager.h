#pragma once
#include "Asset.h"
#include "Nova/Graphics/Resources/Material/Material.h"
#include "AssetFormats/MaterialAsset.h"

namespace Nova
{
	class ModelSourceAsset;
	class Texture2DAsset;
	struct Texture2DImportSettings;
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

		static const std::filesystem::path& GetAssetsPath();
		static const std::filesystem::path& GetAssetsFullPath();

		template<typename TAsset>
		static std::shared_ptr<TAsset> GetAsset(const std::filesystem::path& assetPath) { static_assert(false, "Unsupported SourceAsset"); }

		template<typename TAsset, typename TSettings>
		static std::shared_ptr<TAsset> GetAsset(const std::filesystem::path& assetPath, const TSettings& settings) { static_assert(false, "Unsupported SourceAsset"); }

		template<>
		static std::shared_ptr<ModelSourceAsset> GetAsset<ModelSourceAsset>(const std::filesystem::path& assetPath);

		template<>
		static std::shared_ptr<Texture2DAsset> GetAsset<Texture2DAsset>(const std::filesystem::path& assetPath);

		template<>
		static std::shared_ptr<Texture2DAsset> GetAsset<Texture2DAsset, Texture2DImportSettings>(const std::filesystem::path& assetPath, const Texture2DImportSettings& settings);

		template<>
		static std::shared_ptr<SkyboxAsset> GetAsset<SkyboxAsset>(const std::filesystem::path& assetPath);

		template<>
		static std::shared_ptr<ShaderAsset> GetAsset<ShaderAsset>(const std::filesystem::path& assetPath);


		template<AssetType TAsset>
		static std::shared_ptr<TAsset> GetAsset(const AssetID& assetID);

		template<AssetType TAsset, typename... Args>
		static std::shared_ptr<TAsset> CreateAsset(const AssetID& assetID, const std::string& name, Args&&... args);

		template<Graphics::MaterialType TMaterial, typename... Args>
		static std::shared_ptr<MaterialAsset> CreateMaterialAsset(const std::string& name, Args&&... args);

		static bool DestroyAsset(const AssetID& assetID);

	private:
		using AssetRegistry = std::unordered_map<AssetID, std::shared_ptr<Asset>>;
		template<typename T>
		using ToAssetID = std::unordered_map<T, AssetID>;

		static AssetManager& Get();

		template<SourceAssetType TAsset, class Importer>
		static std::shared_ptr<TAsset> LoadFromPath(const std::filesystem::path& assetPath);

		template<SourceAssetType TAsset, class Importer, typename TSettings>
		static std::shared_ptr<TAsset> LoadFromPath(const std::filesystem::path& assetPath, const TSettings& settings);

		AssetRegistry m_AssetRegistry;
		ToAssetID<std::string> m_NameToAssetID;
		ToAssetID<std::filesystem::path> m_PathToAssetID;

		static AssetManager* m_Instance;
	};

	template<AssetType TAsset>
	std::shared_ptr<TAsset> Nova::AssetManager::GetAsset(const AssetID& assetID)
	{
		AssetManager& assetManager = AssetManager::Get();
		auto asset = assetManager.m_AssetRegistry.find(assetID);
		if (asset == assetManager.m_AssetRegistry.end())
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

		AssetManager& assetManager = AssetManager::Get();
		assetManager.m_AssetRegistry.insert_or_assign(asset->GetAssetID(), asset);
		assetManager.m_NameToAssetID.insert_or_assign(asset->GetName(), asset->GetAssetID());
		return asset;
	}

	template<Graphics::MaterialType TMaterial, typename... Args>
	std::shared_ptr<MaterialAsset> AssetManager::CreateMaterialAsset(const std::string& name, Args&&... args)
	{
		auto materialAsset = CreateAsset<MaterialAsset>(AssetID::NewID(), name);
		materialAsset->m_Material = std::make_shared<TMaterial>(std::forward<Args>(args)...);

		return materialAsset;
	}
}
