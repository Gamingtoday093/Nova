#pragma once
#include "Nova/Assets/AssetFormats/ShaderAsset.h"

namespace Nova::Assets
{
	class ShaderImporter
	{
	public:
		static bool Supported(const std::filesystem::path& assetPath);
		static std::shared_ptr<ShaderAsset> LoadFromPath(const std::filesystem::path& assetPath);
	};
}
