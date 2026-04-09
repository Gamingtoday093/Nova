#include "pch.h"
#include "AssetID.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

Nova::AssetID Nova::Assets::NewAssetID()
{
	AssetID assetID;
	NOVA_HRASSERT(CoCreateGuid(&assetID), "Failed to create AssetID");
	return assetID;
}

std::string Nova::Assets::GetStringAssetID(const AssetID& assetID)
{
	return std::format("{:08x}-{:04x}-{:04x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
		assetID.Data1,
		assetID.Data2,
		assetID.Data3,
		assetID.Data4[0], assetID.Data4[1],
		assetID.Data4[2], assetID.Data4[3], assetID.Data4[4], assetID.Data4[5], assetID.Data4[6], assetID.Data4[7]);
}

size_t Nova::AssetIDHasher::operator()(const AssetID& assetId) const noexcept
{
	const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&assetId);
	std::hash<std::uint64_t> hash;
	return hash(p[0]) ^ hash(p[1]);
}
