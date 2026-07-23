#include "novapch.h"
#include "RenderTexture.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include "Nova/Graphics/DX11.h"

Nova::Graphics::RenderTexture::RenderTexture(bool sRGB) : m_sRGB(sRGB) { }

Nova::Graphics::RenderTexture::RenderTexture(uint32_t width, uint32_t height, bool sRGB) : RenderTexture(sRGB)
{
	Resize(width, height);
}

Nova::Graphics::RenderTexture::~RenderTexture()
{
	m_RenderTargetView = nullptr;
	m_TextureView = nullptr;
}

void Nova::Graphics::RenderTexture::Resize(uint32_t width, uint32_t height)
{
	if (m_Width == width && m_Height == height) return;

	m_Width = width;
	m_Height = height;

	if (m_Width == 0 || m_Height == 0) return;

	CreateRenderTargetView();
	CreateDepthStencilView();
}

void Nova::Graphics::RenderTexture::Bind(const float clearColor[4]) const
{
	DX11::GetContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

	DX11::GetContext()->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
	DX11::GetContext()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

float Nova::Graphics::RenderTexture::GetAspectRatio() const
{
	return float(m_Width) / float(m_Height);
}

uint32_t Nova::Graphics::RenderTexture::GetWidth() const
{
	return m_Width;
}

uint32_t Nova::Graphics::RenderTexture::GetHeight() const
{
	return m_Height;
}

ID3D11ShaderResourceView* Nova::Graphics::RenderTexture::GetTexture() const
{
	return m_TextureView.Get();
}

void Nova::Graphics::RenderTexture::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> renderTexture;
	D3D11_TEXTURE2D_DESC renderTextureDesc
	{
		.Width = m_Width,
		.Height = m_Height,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS,
		.SampleDesc
		{
			.Count = 1,
			.Quality = 0
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateTexture2D(&renderTextureDesc, nullptr, &renderTexture), "Create RenderTexture");

	D3D11_RENDER_TARGET_VIEW_DESC textureViewDesc
	{
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
		.Texture2D
		{
			.MipSlice = 0
		}
	};
	NOVA_HRASSERT(DX11::GetDevice()->CreateRenderTargetView(renderTexture.Get(), &textureViewDesc, &m_RenderTargetView), "Create TextureView");

	D3D11_SHADER_RESOURCE_VIEW_DESC textureResourceDesc
	{
		.Format = m_sRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM,
		.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
		.Texture2D
		{
			.MostDetailedMip = 0,
			.MipLevels = 1
		}
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateShaderResourceView(renderTexture.Get(), &textureResourceDesc, &m_TextureView), "Create TextureResource");
}

void Nova::Graphics::RenderTexture::CreateDepthStencilView()
{
	ComPtr<ID3D11Texture2D> depthStencilTexture;
	D3D11_TEXTURE2D_DESC depthTextureDesc
	{
		.Width = m_Width,
		.Height = m_Height,
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_D32_FLOAT,
		.SampleDesc
		{
			.Count = 1,
			.Quality = 0
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL
	};
	NOVA_HRASSERT(DX11::GetDevice()->CreateTexture2D(&depthTextureDesc, nullptr, &depthStencilTexture), "Create DepthStencilTexture");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc
	{
		.Format = DXGI_FORMAT_D32_FLOAT,
		.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
		.Texture2D
		{
			.MipSlice = 0
		}
	};
	NOVA_HRASSERT(DX11::GetDevice()->CreateDepthStencilView(depthStencilTexture.Get(), &depthViewDesc, &m_DepthStencilView), "Create DepthStencilView");
}
