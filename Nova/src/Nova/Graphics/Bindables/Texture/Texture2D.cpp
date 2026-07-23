#include "novapch.h"
#include "Texture2D.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include <DirectXTex/DirectXTex.h>
#include "Nova/Graphics/DX11.h"
#include "Nova/Tools/DirectXExtensions.h"

Nova::Graphics::Texture2D::Texture2D(const DirectX::ScratchImage& image, bool sRGB)
{
	ComPtr<ID3D11Resource> texture;
	NOVA_HRASSERT(DirectX::CreateTextureEx(DX11::GetDevice(),
		image.GetImage(0, 0, 0), 1, image.GetMetadata(),
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		sRGB ? DirectX::CREATETEX_FORCE_SRGB : DirectX::CREATETEX_IGNORE_SRGB,
		&texture), "Create Texture");
	
	D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc
	{
		.Format = GetTextureFormat(texture.Get()),
		.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D
		{
			.MipLevels = 1
		}
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateShaderResourceView(texture.Get(), &textureViewDesc, &m_TextureView), "Create Texture View");
}

void Nova::Graphics::Texture2D::Bind() const
{
	Bind(0);
}

void Nova::Graphics::Texture2D::Bind(uint32_t slot) const
{
	DX11::GetContext()->PSSetShaderResources(slot, 1, m_TextureView.GetAddressOf());
}

ID3D11ShaderResourceView* Nova::Graphics::Texture2D::GetTexture() const
{
	return m_TextureView.Get();
}
