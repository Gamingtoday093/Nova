#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "RenderStates.hpp"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class BlendState : public IBindable
	{
	public:
		void Create(EBlendState blendState = EBlendState::Opaque);

		void Bind() const override;

	private:
		ComPtr<ID3D11BlendState> m_BlendState;
	};
}
