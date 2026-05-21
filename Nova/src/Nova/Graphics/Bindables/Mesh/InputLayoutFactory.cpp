#include "novapch.h"
#include "InputLayoutFactory.h"

namespace Nova::Graphics::InputLayoutFactory
{
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetVertexDescription()
    {
        static const std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDescription =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        return vertexDescription;
    }

    const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetSkyboxVertexDescription()
    {
        static const std::vector<D3D11_INPUT_ELEMENT_DESC> vertexDescription =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        return vertexDescription;
    }

    template<>
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<Vertex>()
    {
        return GetVertexDescription();
    }

    template<>
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetDescription<SkyboxVertex>()
    {
        return GetSkyboxVertexDescription();
    }
}
