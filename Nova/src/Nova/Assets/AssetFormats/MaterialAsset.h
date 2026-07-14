#pragma once
#include "Nova/Assets/Asset.h"
#include "Nova/Graphics/Resources/Material/Material.h"

namespace Nova
{
	class AssetManager;
}

namespace Nova
{
	class MaterialAsset : public SourceAsset
	{
	public:
		ASSET_TYPE(MaterialAsset, EAssetType::Material)

		MaterialAsset(const AssetID& assetID, const std::string& name);
		MaterialAsset(const AssetID& assetID, const std::filesystem::path& assetPath);

		template<Graphics::MaterialType T>
		AssetRef<T> GetMaterial() const;
		AssetRef<Graphics::Material> GetMaterial() const;

	private:
		AssetRef<Graphics::Material> m_Material = nullptr;

		friend class AssetManager;
	};

	template<Graphics::MaterialType T>
	AssetRef<T> MaterialAsset::GetMaterial() const
	{
		return std::static_pointer_cast<T>(m_Material);
	}
}
