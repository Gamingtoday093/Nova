#include "pch.h"
#include "DepthStencilState.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

void Nova::Graphics::DepthStencilState::Create(EDepthMode depthMode)
{
	D3D11_DEPTH_STENCIL_DESC depthDesc
	{
		.StencilEnable = false
	};

	switch (depthMode)
	{
		case EDepthMode::ReadWrite:
			depthDesc.DepthEnable = true;
			depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			break;
		case EDepthMode::ReadOnly:
			depthDesc.DepthEnable = true;
			depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			break;
		case EDepthMode::Disabled:
			depthDesc.DepthEnable = false;
			depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			break;
		default:
			NOVA_ASSERT(false, "Unsupported DepthMode");
			break;
	}

	NOVA_HRASSERT(DX11::GetDevice()->CreateDepthStencilState(&depthDesc, &m_DepthStencilState), "Create Depth Stencil State");
}

void Nova::Graphics::DepthStencilState::Bind() const
{
	NOVA_ASSERT(m_DepthStencilState.Get(), "Depth Stencil State not Created");
	DX11::GetContext()->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);
}
