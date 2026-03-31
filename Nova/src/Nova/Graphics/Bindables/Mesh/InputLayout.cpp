#include "pch.h"
#include "InputLayout.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include <d3dcompiler.h>

void Nova::Graphics::InputLayout::Create(const std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexDescription, const std::string& shaderFileName)
{
	// Validate Description with Vertex Shader
	ComPtr<ID3DBlob> vertexShader;
	NOVA_HRASSERT(D3DReadFileToBlob(ToWString("CompiledShaders/", shaderFileName).c_str(), &vertexShader), "Failed to Read Vertex Shader");

	NOVA_HRASSERT(DX11::GetDevice()->CreateInputLayout(
		std::data(vertexDescription), UINT(vertexDescription.size()),
		vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(),
		&m_InputLayout), "Create Input Layout");
}

void Nova::Graphics::InputLayout::Bind() const
{
	NOVA_ASSERT(m_InputLayout.Get(), "Input Layout not Created");
	DX11::GetContext()->IASetInputLayout(m_InputLayout.Get());
}
