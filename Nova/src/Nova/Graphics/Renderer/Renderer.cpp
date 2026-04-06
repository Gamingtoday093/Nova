#include "pch.h"
#include "Renderer.h"
#include "Nova/Graphics/Bindables/Mesh/IndexBuffer.h"
#include "Nova/Graphics/Bindables/Mesh/InputLayout.h"
#include "Nova/Input/Input.h"

Nova::Graphics::Renderer::Renderer(DX11& framework) : m_Framework(framework),
	m_TransformBuffer(EBindType::VertexShader), m_AnimationBuffer(EBindType::VertexShader, 256)
{
	DX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_DepthStencilState.Create(EDepthMode::ReadWrite);
	m_Rasterizer.Create(ECullMode::Back);

	for (size_t i = 0; i < m_BlendStates.size(); i++)
		m_BlendStates[i].Create(static_cast<EBlendState>(i));

	constexpr float side = 1.0f / 2.0f;

	std::vector<Vertex> vertices;

	vertices.push_back({ -side, -side, -side });
	vertices.push_back({ side, -side, -side });
	vertices.push_back({ -side, side, -side });
	vertices.push_back({ side, side, -side });
	vertices.push_back({ -side, -side, side });
	vertices.push_back({ side, -side, side });
	vertices.push_back({ -side, side, side });
	vertices.push_back({ side, side, side });

	m_VertexBuffer.Create(vertices);

	m_IndexBuffer.Create({ 0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4 });

	m_InputLayout.Create<Vertex>("VertexShader_vs");
	
	m_VertexShader.Create("VertexShader_vs");
	m_PixelShader.Create("PixelShader_ps");
	
	m_VertexShader.Bind();
	m_PixelShader.Bind();

	m_InputLayout.Bind();
	m_DepthStencilState.Bind();
	m_Rasterizer.Bind();

	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();
}

void Nova::Graphics::Renderer::RenderCube(Camera& camera)
{
	m_TransformBuffer.Data.ProjectionViewMatrix =
		camera.CalculateViewMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(camera.FovAngle, m_Framework.GetAspectRatio(), camera.NearClipPlane, camera.FarClipPlane);

	m_TransformBuffer.Data.ModelMatrix =
		DirectX::XMMatrixIdentity();

	m_TransformBuffer.ApplyBuffer();
	m_TransformBuffer.Bind();

	DX11::GetContext()->DrawIndexed(m_IndexBuffer.Length(), 0, 0);
}
