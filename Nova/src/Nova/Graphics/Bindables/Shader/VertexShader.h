#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class VertexShader : public IBindable
	{
	public:
		void Create(const std::string& shaderName);
		void Bind() const override;

	private:
		ComPtr<ID3D11VertexShader> m_VertexShader;
	};
}
