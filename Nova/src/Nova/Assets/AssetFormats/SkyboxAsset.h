#pragma once
#include "Nova/Assets/Asset.h"

namespace Nova::Graphics
{
	class CubeTexture;
}

namespace Nova::Assets
{
	class SkyboxImporter;
}

namespace Nova
{
	class SkyboxAsset : public SourceAsset
	{
	public:
		ASSET_TYPE(SkyboxAsset, EAssetType::Skybox)

		SkyboxAsset(const AssetID& assetID, const std::filesystem::path& assetPath);
		~SkyboxAsset();

		const Graphics::CubeTexture& GetSkyboxTexture() const;

	private:
		std::unique_ptr<Graphics::CubeTexture> m_SkyboxTexure;

		friend class Assets::SkyboxImporter;
	};
}
