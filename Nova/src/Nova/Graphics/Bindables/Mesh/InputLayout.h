#pragma once
#include "Nova/Graphics/Bindables/IBindable.hpp"
#include "Nova/Graphics/RenderData/VertexFormats.h"
#include "InputLayoutFactory.h"

using namespace Microsoft::WRL;

namespace Nova::Graphics
{
	class InputLayout : public IBindable
	{
	public:
		template<VertexFormat TVertex>
		void Create(const std::string& shaderName);
		void Bind() const override;

	private:
		void Create(const std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexDescription, const std::string& shaderFileName);

		ComPtr<ID3D11InputLayout> m_InputLayout;
	};

	template<VertexFormat TVertex>
	void InputLayout::Create(const std::string& shaderName)
	{
		Create(InputLayoutFactory::GetDescription<TVertex>(), shaderName + ".cso");
	}
}
