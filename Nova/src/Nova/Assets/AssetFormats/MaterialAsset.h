#pragma once
#include "Nova/Assets/Asset.h"
#include "Nova/Graphics/Resources/Material/Material.h"

namespace Nova
{
	class MaterialAsset : public SourceAsset
	{
	public:
		ASSET_TYPE(MaterialAsset, EAssetType::Material)

		MaterialAsset(const AssetID& assetID, const std::filesystem::path& assetPath);

		template<Graphics::MaterialType T, typename... Args>
		std::shared_ptr<T> CreateMaterial(Args&&... args);

		std::shared_ptr<Graphics::Material> GetMaterial() const;

	private:
		std::shared_ptr<Graphics::Material> m_Material;
	};

	template<Graphics::MaterialType T, typename... Args>
	std::shared_ptr<T> MaterialAsset::CreateMaterial(Args&&... args)
	{
		return std::static_pointer_cast<T>(m_Material = std::make_shared<T>(std::forward<Args>(args)...));
	}
}
