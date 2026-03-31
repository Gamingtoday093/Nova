#include "pch.h"
#include "VertexShader.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include <d3dcompiler.h>

void Nova::Graphics::VertexShader::Create(const std::string& shaderName)
{
	ComPtr<ID3DBlob> vertexShader;
	NOVA_HRASSERT(D3DReadFileToBlob(ToWString("Assets/CompiledShaders/" + shaderName + ".cso").c_str(), &vertexShader), "Failed to Read Vertex Shader");

	NOVA_HRASSERT(DX11::GetDevice()->CreateVertexShader(
		vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(),
		nullptr,
		&m_VertexShader), "Create Vertex Shader");
}

void Nova::Graphics::VertexShader::Bind() const
{
	NOVA_ASSERT(m_VertexShader.Get(), "Vertex Shader not Created");
	DX11::GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
}
