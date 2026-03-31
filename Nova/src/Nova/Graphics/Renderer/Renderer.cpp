#include "pch.h"
#include "Renderer.h"
#include "Nova/Graphics/Bindables/Mesh/IndexBuffer.h"
#include "Nova/Graphics/Bindables/Mesh/InputLayout.h"

Nova::Graphics::Renderer::Renderer(DX11& framework) : m_Framework(framework), m_EvilGuy(EBindType::VertexShader), m_AnimationBuffer(EBindType::VertexShader, 256)
{
	m_DepthStencilState.Create(EDepthMode::ReadWrite);
	m_Rasterizer.Create(ECullMode::Back);

	for (size_t i = 0; i < m_BlendStates.size(); i++)
		m_BlendStates[i].Create(static_cast<EBlendState>(i));
}
