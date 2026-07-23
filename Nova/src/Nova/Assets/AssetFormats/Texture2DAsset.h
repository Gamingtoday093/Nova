#pragma once
#include "Nova/Assets/Asset.h"

namespace Nova::Graphics
{
	class Texture2D;
}

namespace Nova::Assets
{
	class TextureImporter;
}

namespace Nova
{
	struct Texture2DImportSettings
	{
		bool ForceSRGB = true;

		bool operator==(const Texture2DImportSettings& other) const;
	};

	class Texture2DAsset : public SourceAsset
	{
	public:
		ASSET_TYPE(Texture2DAsset, EAssetType::Texture)

		Texture2DAsset(const AssetID& assetID, const std::filesystem::path& assetPath);
		~Texture2DAsset();

		const Texture2DImportSettings& GetSettings() const;
		void SetSettings(const Texture2DImportSettings& settings);

		AssetRef<Graphics::Texture2D> GetTexture() const;

	private:
		Texture2DImportSettings m_Settings;

		AssetRef<Graphics::Texture2D> m_Texture = nullptr;

		friend class Assets::TextureImporter;
	};
}

