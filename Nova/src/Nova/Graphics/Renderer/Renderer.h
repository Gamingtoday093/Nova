#pragma once
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Bindables/Render/DepthStencilState.h"
#include "Nova/Graphics/Bindables/Render/Rasterizer.h"
#include "Nova/Graphics/Bindables/Render/BlendState.h"
#include "Nova/Graphics/Bindables/Shader/StructBuffer.hpp"
#include "Nova/Graphics/Bindables/Shader/ConstBuffer.hpp"
#include <DirectXMath.h>
#include "Nova/Graphics/RenderData/BufferStructs.h"
#include "Nova/Graphics/Bindables/Mesh/VertexBuffer.h"
#include "Nova/Graphics/Bindables/Mesh/IndexBuffer.h"
#include "Nova/Graphics/Bindables/Shader/VertexShader.h"
#include "Nova/Graphics/Bindables/Shader/Sampler.h"
#include "Nova/Graphics/Bindables/Shader/PixelShader.h"
#include "Nova/Graphics/Bindables/Mesh/InputLayout.h"
#include "Nova/Scene/Camera/Camera.hpp"
#include "Nova/Graphics/Resources/Mesh.h"
#include "Nova/Graphics/Resources/SkyboxCube.h"

namespace Nova::Graphics
{
	class CubeTexture;
	class Transform;
}

namespace Nova::Graphics
{
	class Renderer
	{
	public:
		Renderer(DX11& framework);

		void RenderCube(const Camera& camera);
		void RenderShip(const Camera& camera);
		void RenderModel(const Transform& transform, const Mesh& mesh, const Camera& camera);
		void RenderSkybox(const CubeTexture& skyboxTexture, const Camera& camera);

	private:
		DX11& m_Framework;

		SkyboxCube m_SkyboxMesh;
		InputLayout m_SkyboxLayout;
		VertexShader m_SkyboxVertexShader;
		Sampler m_SkyboxSampler;
		PixelShader m_SkyboxShader;
		Rasterizer m_SkyboxRasterizer;
		DepthStencilState m_SkyboxDepth;

		std::shared_ptr<Mesh> m_ShipMesh;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		InputLayout m_InputLayout;
		VertexShader m_VertexShader;
		PixelShader m_PixelShader;

		ConstBuffer<TransformBuffer> m_TransformBuffer;
		StructBuffer<DirectX::XMMATRIX> m_AnimationBuffer;

		DepthStencilState m_DepthStencilState;
		Rasterizer m_Rasterizer;
		std::array<BlendState, size_t(EBlendState::Count)> m_BlendStates;
	};
}
