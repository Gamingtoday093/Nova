#include "novapch.h"
#include "SkyboxAsset.h"
#include "Nova/Graphics/Bindables/Texture/CubeTexture.h"

Nova::SkyboxAsset::SkyboxAsset(const AssetID & assetID, const std::filesystem::path & assetPath) : SourceAsset(assetID, assetPath) { }

Nova::SkyboxAsset::~SkyboxAsset() = default;

const Nova::Graphics::CubeTexture& Nova::SkyboxAsset::GetSkyboxTexture() const
{
	return *m_SkyboxTexure;
}
