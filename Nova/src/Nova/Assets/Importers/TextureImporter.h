#pragma once
#include "Nova/Assets/AssetFormats/Texture2DAsset.h"

namespace Nova::Assets
{
	class TextureImporter
	{
	public:
		static bool Supported(const std::filesystem::path& assetPath);
		static std::shared_ptr<Texture2DAsset> LoadFromPath(const std::filesystem::path& assetPath, const Texture2DImportSettings& settings);
	};
}
