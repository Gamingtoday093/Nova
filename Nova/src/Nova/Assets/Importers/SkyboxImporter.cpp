#include "novapch.h"
#include "SkyboxImporter.h"
#include <DirectXTex/DirectXTex.h>
#include "Nova/Graphics/Bindables/Texture/CubeTexture.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

bool Nova::Assets::SkyboxImporter::Supported(const std::filesystem::path& assetPath)
{
    if (!std::filesystem::is_directory(assetPath)) return false;

    for (size_t img = 0; img < 6; img++)
        if (!std::filesystem::exists(assetPath / (std::to_string(img) + ".png"))) return false;

    return true;
}

std::shared_ptr<Nova::SkyboxAsset> Nova::Assets::SkyboxImporter::LoadFromPath(const std::filesystem::path& assetPath)
{
    // Load .asset to store AssetID

    // Maybe faster to store in .cache?
    std::vector<DirectX::ScratchImage> images;
    images.resize(6);
    for (size_t img = 0; img < 6; img++)
    {
        NOVA_HRASSERT(DirectX::LoadFromWICFile((assetPath / (std::to_string(img) + ".png")).wstring().c_str(), DirectX::WIC_FLAGS_NONE, nullptr, images[img]), "Load Skybox Image");
    }

    std::shared_ptr<SkyboxAsset> asset = std::make_shared<SkyboxAsset>(AssetID::NewID(), assetPath);
    asset->m_SkyboxTexure = std::make_unique<Graphics::CubeTexture>(images);

    return asset;
}
