#include "pch.h"
#include "BlendState.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"

void Nova::Graphics::BlendState::Create(EBlendState blendState)
{
	D3D11_BLEND_DESC blendDesc
	{
		.AlphaToCoverageEnable = false,
		.IndependentBlendEnable = false,
	};

	auto& renderTarget = blendDesc.RenderTarget[0];
	renderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	switch (blendState)
	{
		case EBlendState::Opaque:
			renderTarget.BlendEnable = false;
			// final = src * 1 + dest * 0
			renderTarget.SrcBlend = D3D11_BLEND_ONE;
			renderTarget.DestBlend = D3D11_BLEND_ZERO;
			renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

			renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
			renderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
			renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
		case EBlendState::Alpha:
			renderTarget.BlendEnable = true;
			// final = src * src.a + dest * (1 - src.a)
			renderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			renderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

			renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
			renderTarget.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
		case EBlendState::Additive:
			renderTarget.BlendEnable = true;
			// final = src * src.a + dst * 1;
			renderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			renderTarget.DestBlend = D3D11_BLEND_ONE;
			renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

			renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
			renderTarget.DestBlendAlpha = D3D11_BLEND_ONE;
			renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
		case EBlendState::Multiply:
			renderTarget.BlendEnable = true;
			// final = src * 0 + dst * src
			renderTarget.SrcBlend = D3D11_BLEND_ZERO;
			renderTarget.DestBlend = D3D11_BLEND_SRC_COLOR;
			renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

			renderTarget.SrcBlendAlpha = D3D11_BLEND_ZERO;
			renderTarget.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
		case EBlendState::Premultiplied:
			renderTarget.BlendEnable = true;
			// final = src * 1 + dst * (1 - src.a)
			renderTarget.SrcBlend = D3D11_BLEND_ONE;
			renderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			renderTarget.BlendOp = D3D11_BLEND_OP_ADD;

			renderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
			renderTarget.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			renderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			break;
		default:
			NOVA_ASSERT(false, "Unsupported BlendState");
			break;
	}

	NOVA_HRASSERT(DX11::GetDevice()->CreateBlendState(&blendDesc, &m_BlendState), "Create Blend State");
}

void Nova::Graphics::BlendState::Bind() const
{
	NOVA_ASSERT(m_BlendState.Get(), "Blend State not Created");
	static constexpr float blendFactor[4] = { 0, 0, 0, 0 };
	DX11::GetContext()->OMSetBlendState(m_BlendState.Get(), blendFactor, 0xffffffff);
}
