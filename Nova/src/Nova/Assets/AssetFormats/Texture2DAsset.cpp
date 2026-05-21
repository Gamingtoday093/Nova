#include "novapch.h"
#include "Texture2DAsset.h"
#include "Nova/Graphics/Bindables/Texture/Texture2D.h"

bool Nova::Texture2DImportSettings::operator==(const Texture2DImportSettings& other) const
{
	return memcmp(this, &other, sizeof(Texture2DImportSettings)) == 0;
}

Nova::Texture2DAsset::Texture2DAsset(const AssetID& assetID, const std::filesystem::path& assetPath) : SourceAsset(assetID, assetPath) { }

Nova::Texture2DAsset::~Texture2DAsset() = default;

const Nova::Texture2DImportSettings& Nova::Texture2DAsset::GetSettings() const
{
	return m_Settings;
}

void Nova::Texture2DAsset::SetSettings(const Texture2DImportSettings& settings)
{
	bool needsReload = m_Settings != settings;
	m_Settings = settings;
	if (needsReload) NOVA_CORE_WARN("Texture2DAsset needs Reload!");
}

std::shared_ptr<Nova::Graphics::Texture2D> Nova::Texture2DAsset::GetTexture() const
{
	NOVA_ASSERT(m_Texture, "Invalid Texture2DAsset");
	return m_Texture;
}
