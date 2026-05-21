#include "novapch.h"
#include "MaterialAsset.h"

Nova::MaterialAsset::MaterialAsset(const AssetID& assetID, const std::filesystem::path& assetPath) : SourceAsset(assetID, assetPath) { }

std::shared_ptr<Nova::Graphics::Material> Nova::MaterialAsset::GetMaterial() const
{
	NOVA_ASSERT(m_Material, "Invalid MaterialAsset");
	return m_Material;
}
