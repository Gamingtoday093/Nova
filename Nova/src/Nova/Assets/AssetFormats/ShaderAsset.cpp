#include "novapch.h"
#include "ShaderAsset.h"
#include "Nova/Graphics/Bindables/Shader/PixelShader.h"

Nova::ShaderAsset::ShaderAsset(const AssetID& assetID, const std::filesystem::path& assetPath) : SourceAsset(assetID, assetPath) { }

Nova::ShaderAsset::~ShaderAsset() = default;

Nova::AssetRef<Nova::Graphics::PixelShader> Nova::ShaderAsset::GetShader()
{
	NOVA_ASSERT(m_Shader, "Invalid ShaderAsset");
	return m_Shader;
}
