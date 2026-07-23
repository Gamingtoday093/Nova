#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"

namespace DirectX
{
	class ScratchImage;
}

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class Texture2D : IBindable
	{
	public:
		Texture2D(const DirectX::ScratchImage& image, bool sRGB = true);

		void Bind() const override;
		void Bind(uint32_t slot) const;

		ID3D11ShaderResourceView* GetTexture() const;

	private:
		ComPtr<ID3D11ShaderResourceView> m_TextureView;
	};
}

