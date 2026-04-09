#pragma once

namespace Nova
{
	using AssetID = GUID;

	namespace Assets
	{
		AssetID NewAssetID();
		std::string GetStringAssetID(const AssetID& assetID);
	}

	// https://stackoverflow.com/questions/24113864/what-is-the-right-way-to-use-a-guid-as-the-key-in-stdhash-map
	struct AssetIDHasher
	{
		size_t operator()(const AssetID& assetId) const noexcept;
	};
}