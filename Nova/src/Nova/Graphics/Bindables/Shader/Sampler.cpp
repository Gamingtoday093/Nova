#include "novapch.h"
#include "Sampler.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include "Nova/Graphics/DX11.h"

Nova::Graphics::Sampler::Sampler()
{
	D3D11_SAMPLER_DESC samplerDesc
	{
		.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		.AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
		.AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
		.AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
	};

	NOVA_HRASSERT(DX11::GetDevice()->CreateSamplerState(&samplerDesc, &m_SamplerState), "Create Sampler State");
}

void Nova::Graphics::Sampler::Bind() const
{
	Bind(0);
}

void Nova::Graphics::Sampler::Bind(uint32_t slot) const
{
	DX11::GetContext()->PSSetSamplers(slot, 1, m_SamplerState.GetAddressOf());
}
