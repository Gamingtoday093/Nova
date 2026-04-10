#include "pch.h"
#include "Asset.h"

Nova::Asset::Asset(const AssetID& assetID, const std::string& name)
{
    m_AssetID = assetID;

    m_Name = name;
}

const Nova::AssetID& Nova::Asset::GetAssetID() const
{
    return m_AssetID;
}

const std::string& Nova::Asset::GetName() const
{
    return m_Name;
}

Nova::SourceAsset::SourceAsset(const AssetID& assetID, const std::filesystem::path& assetPath) : Asset(assetID, assetPath.stem().string())
{
    m_AssetPath = assetPath;
}

const std::filesystem::path& Nova::SourceAsset::GetAssetPath() const
{
    return m_AssetPath;
}
