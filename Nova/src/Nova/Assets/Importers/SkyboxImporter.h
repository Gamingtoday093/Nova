#pragma once
#include "Nova/Assets/AssetFormats/SkyboxAsset.h"

namespace Nova::Assets
{
	class SkyboxImporter
	{
	public:
		static bool Supported(const std::filesystem::path& assetPath);
		static std::shared_ptr<SkyboxAsset> LoadFromPath(const std::filesystem::path& assetPath);
	};
}
