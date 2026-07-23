#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace Nova::Graphics
{
	class RenderTexture
	{
	public:
		RenderTexture(bool sRGB = true);
		RenderTexture(uint32_t width, uint32_t height, bool sRGB = true);
		~RenderTexture();
		
		void Resize(uint32_t width, uint32_t height);
		void Bind(const float clearColor[4]) const;

		float GetAspectRatio() const;
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		ID3D11ShaderResourceView* GetTexture() const;

	private:
		uint32_t m_Width = 0, m_Height = 0;
		bool m_sRGB;

		void CreateRenderTargetView();
		void CreateDepthStencilView();

		ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		ComPtr<ID3D11ShaderResourceView> m_TextureView;
	};
}

