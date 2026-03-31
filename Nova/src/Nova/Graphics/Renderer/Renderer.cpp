#include "pch.h"
#include "Renderer.h"

Nova::Graphics::Renderer::Renderer(DX11& framework) : m_Framework(framework)
{
	m_DepthStencilState.Create();
	m_Rasterizer.Create();

	for (size_t i = 0; i < m_BlendStates.size(); i++)
		m_BlendStates[i].Create(static_cast<EBlendState>(i));
}
