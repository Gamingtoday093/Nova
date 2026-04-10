#pragma once
#include "Nova/Graphics/RenderData/VertexFormats.h"

namespace Nova::Graphics::InputLayoutFactory
{
	template<typename TVertex>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription() { static_assert(false); }

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<Vertex>();

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<SkyboxVertex>();

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetVertexDescription();

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetSkyboxVertexDescription();
}
