#include "novapch.h"
#include "TextureImporter.h"
#include <DirectXTex/DirectXTex.h>
#include "Nova/Graphics/Bindables/Texture/Texture2D.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

bool Nova::Assets::TextureImporter::Supported(const std::filesystem::path& assetPath)
{
	if (!assetPath.has_extension()) return false;

	static std::initializer_list<std::string> supportedFileTypes = { ".jpg", ".png" };
	std::string extension = assetPath.extension().string();

	for (auto& fileType : supportedFileTypes)
		if (extension == fileType) return true;

	return false;
}

std::shared_ptr<Nova::Texture2DAsset> Nova::Assets::TextureImporter::LoadFromPath(const std::filesystem::path& assetPath)
{
	DirectX::ScratchImage image;
	NOVA_HRASSERT(DirectX::LoadFromWICFile((assetPath).wstring().c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image), "Load Skybox Image");

	std::shared_ptr<Texture2DAsset> asset = std::make_shared<Texture2DAsset>(AssetID::NewID(), assetPath);
	asset->m_Texture = std::make_unique<Graphics::Texture2D>(image);

	return asset;
}
