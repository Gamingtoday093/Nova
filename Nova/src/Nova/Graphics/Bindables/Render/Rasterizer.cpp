#include "pch.h"
#include "Rasterizer.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

void Nova::Graphics::Rasterizer::Create(ECullMode cullMode)
{
	D3D11_RASTERIZER_DESC rasterizerDesc
	{
		.FillMode = D3D11_FILL_SOLID
	};

	switch (cullMode)
	{
		case ECullMode::Back:
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			break;
		case ECullMode::Front:
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			break;
		case ECullMode::None:
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			break;
		default:
			NOVA_ASSERT(false, "Unsupported CullMode");
			break;
	}

	NOVA_HRASSERT(DX11::GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState), "Create Rasterizer State");
}

void Nova::Graphics::Rasterizer::Bind() const
{
	NOVA_ASSERT(m_RasterizerState.Get(), "Rasterizer State not Created");
	DX11::GetContext()->RSSetState(m_RasterizerState.Get());
}
