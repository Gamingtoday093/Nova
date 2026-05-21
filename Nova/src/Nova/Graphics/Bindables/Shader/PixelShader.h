#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class PixelShader : public IBindable
	{
	public:
		PixelShader() = default;
		PixelShader(const std::filesystem::path& shaderPath);

		void Create(const std::string& shaderName);
		void Bind() const override;

	private:
		void Create(LPCWSTR shaderPath);

		ComPtr<ID3D11PixelShader> m_PixelShader;
	};
}
