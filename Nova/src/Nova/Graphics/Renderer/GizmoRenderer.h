#pragma once
#include "Nova/Graphics/Bindables/Line/LineBuffer.h"
#include "Nova/Graphics/Bindables/Mesh/InputLayout.h"
#include "Nova/Graphics/Bindables/Shader/PixelShader.h"
#include "Nova/Graphics/Bindables/Render/BlendState.h"
#include "Nova/Graphics/Bindables/Shader/VertexShader.h"
#include "Nova/Graphics/Bindables/Shader/ConstBuffer.hpp"
#include "Nova/Graphics/RenderData/BufferStructs.h"

namespace Nova
{
	class Camera;
}

namespace Nova::Graphics
{
	class DX11;
}

namespace Nova::Graphics
{
	class GizmoRenderer
	{
	public:
		GizmoRenderer(const DX11& framework);
		~GizmoRenderer();

		static void RenderLine(XMFLOAT3 startPosition, XMFLOAT3 endPosition, XMFLOAT4 color = { 0, 1, 0, 1 });
		static void RenderPlane(XMFLOAT3 position, XMFLOAT2 scale, XMFLOAT4 color = { 0, 1, 0, 1 });
		static void RenderCube(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT4 color = { 0, 1, 0, 1 });

		void ExecuteCommands(const Camera& camera);

	private:
		static GizmoRenderer& Get();
		
		LineBuffer m_LineBuffer;
		InputLayout m_InputLayout;

		VertexShader m_VertexShader;
		ConstBuffer<ProjectionBuffer> m_ProjectionBuffer;

		PixelShader m_PixelShader;
		BlendState m_BlendState;

		const DX11& m_Framework;
		static GizmoRenderer* m_Instance;
	};
}
