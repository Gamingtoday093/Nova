#pragma once
#include "Nova/Assets/Asset.h"

namespace Nova::Assets
{
	class ShaderImporter;
}

namespace Nova::Graphics
{
	class PixelShader;
}

namespace Nova
{
	class ShaderAsset : public SourceAsset
	{
	public:
		ASSET_TYPE(ShaderAsset, EAssetType::Shader)

		ShaderAsset(const AssetID& assetID, const std::filesystem::path& assetPath);
		~ShaderAsset();

		AssetRef<Graphics::PixelShader> GetShader();

	private:
		AssetRef<Graphics::PixelShader> m_Shader = nullptr;

		friend class Assets::ShaderImporter;
	};
}

