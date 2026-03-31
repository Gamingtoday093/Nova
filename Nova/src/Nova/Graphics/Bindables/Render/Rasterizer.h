#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "RenderStates.hpp"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class Rasterizer : public IBindable
	{
	public:
		void Create(ECullMode cullMode = ECullMode::Back);

		void Bind() const override;

	private:
		ComPtr<ID3D11RasterizerState> m_RasterizerState;
	};
}

