#include "novapch.h"
#include "GizmoRenderer.h"
#include "Nova/Scene/Camera/Camera.h"

Nova::Graphics::GizmoRenderer* Nova::Graphics::GizmoRenderer::m_Instance;

Nova::Graphics::GizmoRenderer::GizmoRenderer(const DX11& framework) : m_Framework(framework),
	m_LineBuffer(LineBuffer::MAX_LINES), m_ProjectionBuffer(Graphics::EBindType::VertexShader)
{
	NOVA_ASSERT(!m_Instance, "Creating multiple GizmoRenderer is not allowed");
	m_Instance = this;

	m_InputLayout.Create<GizmoVertex>("GizmoShader_vs");

	m_VertexShader.Create("GizmoShader_vs");

	m_PixelShader.Create("GizmoShader_ps");
	m_BlendState.Create(Graphics::EBlendState::Alpha);
}

Nova::Graphics::GizmoRenderer::~GizmoRenderer()
{
	m_Instance = nullptr;
}

void Nova::Graphics::GizmoRenderer::RenderLine(XMFLOAT3 startPosition, XMFLOAT3 endPosition, XMFLOAT4 color)
{
	Get().m_LineBuffer.Lines.emplace_back(startPosition, endPosition, color);
}

void Nova::Graphics::GizmoRenderer::RenderPlane(XMFLOAT3 position, XMFLOAT2 scale, XMFLOAT4 color)
{
	RenderLine({ position.x + scale.x, position.y, position.z + scale.y }, { position.x - scale.x, position.y, position.z + scale.y }, color);
	RenderLine({ position.x + scale.x, position.y, position.z + scale.y }, { position.x + scale.x, position.y, position.z - scale.y }, color);;
	RenderLine({ position.x - scale.x, position.y, position.z - scale.y }, { position.x - scale.x, position.y, position.z + scale.y }, color);;
	RenderLine({ position.x - scale.x, position.y, position.z - scale.y }, { position.x + scale.x, position.y, position.z - scale.y }, color);;
}

void Nova::Graphics::GizmoRenderer::RenderCube(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT4 color)
{
	RenderPlane({ position.x, position.y + scale.y, position.z }, { scale.x, scale.z }, color);
	RenderPlane({ position.x, position.y - scale.y, position.z }, { scale.x, scale.z }, color);
	RenderLine({ position.x + scale.x, position.y - scale.y, position.z + scale.z }, { position.x + scale.x, position.y + scale.y, position.z + scale.z }, color);
	RenderLine({ position.x - scale.x, position.y - scale.y, position.z + scale.z }, { position.x - scale.x, position.y + scale.y, position.z + scale.z }, color);
	RenderLine({ position.x + scale.x, position.y - scale.y, position.z - scale.z }, { position.x + scale.x, position.y + scale.y, position.z - scale.z }, color);
	RenderLine({ position.x - scale.x, position.y - scale.y, position.z - scale.z }, { position.x - scale.x, position.y + scale.y, position.z - scale.z }, color);
}

void Nova::Graphics::GizmoRenderer::ExecuteCommands(const Camera& camera)
{
	if (m_LineBuffer.Lines.empty()) return;

	std::vector<Line> queuedLines;
	if (m_LineBuffer.Lines.size() > m_LineBuffer.GetMaxLines())
	{
		queuedLines.reserve(m_LineBuffer.Lines.size() - m_LineBuffer.GetMaxLines());
		queuedLines.insert(queuedLines.begin(), m_LineBuffer.Lines.begin() + m_LineBuffer.GetMaxLines(), m_LineBuffer.Lines.end());
		m_LineBuffer.Lines.resize(m_LineBuffer.GetMaxLines());
	}

	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
	DX11::GetContext()->IAGetPrimitiveTopology(&primitiveTopology);
	DX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	m_InputLayout.Bind();

	m_VertexShader.Bind();

	m_ProjectionBuffer.Data.ProjectionViewMatrix =
		camera.GetViewMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(camera.FovAngle, m_Framework.GetAspectRatio(), camera.NearClipPlane, camera.FarClipPlane);
	m_ProjectionBuffer.ApplyBuffer();
	m_ProjectionBuffer.Bind();

	m_PixelShader.Bind();
	m_BlendState.Bind();

ApplyLineBuffer:
	m_LineBuffer.ApplyChanges();
	m_LineBuffer.Bind();

	DX11::GetContext()->DrawIndexed(uint32_t(m_LineBuffer.Lines.size() * 2), 0, 0);
	m_LineBuffer.Lines.clear();
	if (!queuedLines.empty())
	{
		if (queuedLines.size() > m_LineBuffer.GetMaxLines())
		{
			m_LineBuffer.Lines.insert(m_LineBuffer.Lines.begin(), queuedLines.end() - m_LineBuffer.GetMaxLines(), queuedLines.end());
			queuedLines.resize(queuedLines.size() - m_LineBuffer.GetMaxLines());
		}
		else
		{
			m_LineBuffer.Lines.insert(m_LineBuffer.Lines.begin(), queuedLines.begin(), queuedLines.end());
			queuedLines.clear();
		}
		goto ApplyLineBuffer;
	}

	DX11::GetContext()->IASetPrimitiveTopology(primitiveTopology);
}

Nova::Graphics::GizmoRenderer& Nova::Graphics::GizmoRenderer::Get()
{
	NOVA_ASSERT(m_Instance, "GizmoRenderer hasn't been Initialized");
	return *m_Instance;
}
