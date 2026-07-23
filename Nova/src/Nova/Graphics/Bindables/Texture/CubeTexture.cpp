#include "novapch.h"
#include "CubeTexture.h"
#include <DirectXTex/DirectXTex.h>
#include "Nova/Graphics/Logging/HRAsserts.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Tools/DirectXExtensions.h"

static constexpr size_t TOTAL_TEXTURES = 6;

Nova::Graphics::CubeTexture::CubeTexture(const std::vector<DirectX::ScratchImage>& images)
{
	NOVA_ASSERT(images.size() == TOTAL_TEXTURES, "Cube Texture requires 6 images");

	DirectX::Image imageTextures[TOTAL_TEXTURES] = {};
	for (size_t i = 0; i < TOTAL_TEXTURES; i++)
		imageTextures[i] = *images[i].GetImage(0, 0, 0);

	DirectX::TexMetadata imageMetadata = images[0].GetMetadata();
	imageMetadata.arraySize = TOTAL_TEXTURES;
	imageMetadata.miscFlags |= DirectX::TEX_MISC_TEXTURECUBE;

	ComPtr<ID3D11Resource> texture;
	NOVA_HRASSERT(DirectX::CreateTextureEx(DX11::GetDevice(),
		imageTextures, TOTAL_TEXTURES, imageMetadata,
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		D3D11_RESOURCE_MISC_TEXTURECUBE,
		DirectX::CREATETEX_FORCE_SRGB,
		&texture), "Create Cube Texture");

	D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc
	{
		.Format = GetTextureFormat(texture.Get()),
		.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE,
		.TextureCube
		{
			.MipLevels = 1
		},
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateShaderResourceView(texture.Get(), &textureViewDesc, &m_TextureView), "Create Cube Texture View");
}

void Nova::Graphics::CubeTexture::Bind() const
{
	Bind(0);
}

void Nova::Graphics::CubeTexture::Bind(uint32_t slot) const
{
	DX11::GetContext()->PSSetShaderResources(slot, 1, m_TextureView.GetAddressOf());
}
