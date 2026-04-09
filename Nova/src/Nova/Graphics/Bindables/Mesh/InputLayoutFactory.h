#pragma once
#include "Nova/Graphics/RenderData/VertexFormats.h"

namespace Nova::Graphics::InputLayoutFactory
{
	template<typename TVertex>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription() { static_assert(false); }

	template<>
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<Vertex>();

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetVertexDescription();
}
