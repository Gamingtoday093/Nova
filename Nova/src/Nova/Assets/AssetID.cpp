#include "pch.h"
#include "AssetID.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

Nova::AssetID Nova::AssetID::NewID()
{
	AssetID assetID;
	NOVA_HRASSERT(CoCreateGuid(&assetID), "Failed to create AssetID");
	return assetID;
}

std::string Nova::AssetID::ToString() const
{
	return std::format("{:08x}-{:04x}-{:04x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
		Data1,
		Data2,
		Data3,
		Data4[0], Data4[1],
		Data4[2], Data4[3], Data4[4], Data4[5], Data4[6], Data4[7]);
}

// https://stackoverflow.com/questions/24113864/what-is-the-right-way-to-use-a-guid-as-the-key-in-stdhash-map
size_t std::hash<Nova::AssetID>::operator()(const Nova::AssetID& assetId) const noexcept
{
	const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&assetId);
	std::hash<std::uint64_t> hash;
	return hash(p[0]) ^ hash(p[1]);
}
