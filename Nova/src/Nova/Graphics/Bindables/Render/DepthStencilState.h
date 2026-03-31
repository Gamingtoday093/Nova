#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "RenderStates.hpp"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class DepthStencilState : public IBindable
	{
	public:
		void Create(EDepthMode depthMode = EDepthMode::ReadWrite);

		void Bind() const override;

	private:
		ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	};
}
