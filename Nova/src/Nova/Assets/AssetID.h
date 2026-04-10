#pragma once

namespace Nova
{
	struct AssetID : public GUID
	{
		static AssetID NewID();
		std::string ToString() const;
	};
}

namespace std
{
	template<>
	struct hash<Nova::AssetID>
	{
		size_t operator()(const Nova::AssetID& assetId) const noexcept;
	};
}