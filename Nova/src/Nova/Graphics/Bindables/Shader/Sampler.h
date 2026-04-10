#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class Sampler : public IBindable
	{
	public:
		Sampler();

		void Bind() const override;
		void Bind(uint32_t slot) const;

	private:
		ComPtr<ID3D11SamplerState> m_SamplerState;
	};
}
