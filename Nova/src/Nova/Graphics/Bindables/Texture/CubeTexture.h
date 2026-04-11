#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"

namespace DirectX
{
	class ScratchImage;
}

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class CubeTexture : IBindable
	{
	public:
		CubeTexture(const std::vector<DirectX::ScratchImage>& images);

		void Bind() const override;
		void Bind(uint32_t slot) const;

	private:
		ComPtr<ID3D11ShaderResourceView> m_TextureView;
	};
}
