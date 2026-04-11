#include "novapch.h"
#include "CubeTexture.h"
#include <DirectXTex/DirectXTex.h>
#include "Nova/Graphics/Logging/HRAsserts.h"
#include "Nova/Graphics/DX11.h"

static constexpr size_t TOTAL_TEXTURES = 6;

Nova::Graphics::CubeTexture::CubeTexture(const std::vector<DirectX::ScratchImage>& images)
{
	NOVA_ASSERT(images.size() == TOTAL_TEXTURES, "Cube Texture requires 6 images");

	D3D11_TEXTURE2D_DESC textureDesc
	{
		.Width = uint32_t(images[0].GetMetadata().width),
		.Height = uint32_t(images[0].GetMetadata().height),
		.MipLevels = 1,
		.ArraySize = TOTAL_TEXTURES,
		.Format = images[0].GetMetadata().format,
		.SampleDesc
		{
			.Count = 1,
			.Quality = 0
		},
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_SHADER_RESOURCE,
		.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE
	};

	D3D11_SUBRESOURCE_DATA textureData[TOTAL_TEXTURES] = {};
	for (size_t d = 0; d < TOTAL_TEXTURES; d++)
	{
		textureData[d].pSysMem = images[d].GetPixels();
		textureData[d].SysMemPitch = uint32_t(images[d].GetImage(0, 0, 0)->rowPitch);
		textureData[d].SysMemSlicePitch = 0;
	}

	ComPtr<ID3D11Texture2D> texture;
	NOVA_HRASSERT(DX11::GetDevice()->CreateTexture2D(&textureDesc, textureData, &texture), "Create Cube Texture");

	D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc
	{
		.Format = textureDesc.Format,
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
