#include "novapch.h"
#include "ShaderImporter.h"
#include "Nova/Graphics/Bindables/Shader/PixelShader.h"

bool Nova::Assets::ShaderImporter::Supported(const std::filesystem::path& assetPath)
{
	if (!assetPath.has_extension()) return false;

	static std::initializer_list<std::string> supportedFileTypes = { ".cso" };
	std::string extension = assetPath.extension().string();

	for (auto& fileType : supportedFileTypes)
		if (extension == fileType) return true;

	return false;
}

std::shared_ptr<Nova::ShaderAsset> Nova::Assets::ShaderImporter::LoadFromPath(const std::filesystem::path& assetPath)
{
	std::shared_ptr<ShaderAsset> asset = std::make_shared<ShaderAsset>(AssetID::NewID(), assetPath);

	asset->m_Shader = std::make_shared<Graphics::PixelShader>(assetPath);

    return asset;
}
