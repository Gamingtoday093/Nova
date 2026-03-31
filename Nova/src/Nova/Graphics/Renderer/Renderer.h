#pragma once
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Bindables/Render/DepthStencilState.h"
#include "Nova/Graphics/Bindables/Render/Rasterizer.h"
#include "Nova/Graphics/Bindables/Render/BlendState.h"

namespace Nova::Graphics
{
	class Renderer
	{
	public:
		Renderer(DX11& framework);

	private:
		DX11& m_Framework;

		DepthStencilState m_DepthStencilState;
		Rasterizer m_Rasterizer;
		std::array<BlendState, size_t(EBlendState::Count)> m_BlendStates;
	};
}
