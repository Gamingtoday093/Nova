#include "pch.h"
#include "PixelShader.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Logging/HRAsserts.h"
#include <d3dcompiler.h>

void Nova::Graphics::PixelShader::Create(const std::string& shaderName)
{
	ComPtr<ID3DBlob> pixelShader;
	NOVA_HRASSERT(D3DReadFileToBlob(ToWString("Assets/CompiledShaders/" + shaderName + ".cso").c_str(), &pixelShader), "Failed to Read Pixel Shader");

	NOVA_HRASSERT(DX11::GetDevice()->CreatePixelShader(
		pixelShader->GetBufferPointer(), pixelShader->GetBufferSize(),
		nullptr,
		&m_PixelShader), "Create Pixel Shader");
}

void Nova::Graphics::PixelShader::Bind() const
{
	NOVA_ASSERT(m_PixelShader.Get(), "Pixel Shader not Created");
	DX11::GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}
