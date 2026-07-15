#include "novapch.h"
#include "Renderer.h"
#include "Nova/Assets/AssetFormats/MeshAsset.h"
#include "Nova/Assets/AssetFormats/ModelSourceAsset.h"
#include "Nova/Assets/AssetManager.h"
#include "Nova/Graphics/Bindables/Mesh/IndexBuffer.h"
#include "Nova/Graphics/Bindables/Mesh/InputLayout.h"
#include "Nova/Graphics/Bindables/Texture/CubeTexture.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Resources/Material/Material.h"
#include "Nova/Graphics/Resources/Mesh.h"
#include "Nova/Graphics/Resources/Transform.h"
#include "Nova/Scene/Camera/Camera.h"

Nova::Graphics::Renderer::Renderer(const DX11& framework) : m_Framework(framework),
	m_TransformBuffer(EBindType::VertexShader), m_AnimationBuffer(EBindType::VertexShader, 256)
{
	DX11::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_DepthStencilState.Create(EDepthMode::ReadWrite);
	m_Rasterizer.Create(ECullMode::Back);

	for (size_t i = 0; i < m_BlendStates.size(); i++)
		m_BlendStates[i].Create(static_cast<EBlendState>(i));

	constexpr float side = 1.0f / 2.0f;

	std::vector<Vertex> vertices;

	vertices.push_back({ -side, -side, -side, 0, 0 });
	vertices.push_back({ side, -side, -side, 0, 0 });
	vertices.push_back({ -side, side, -side, 0, 0 });
	vertices.push_back({ side, side, -side, 0, 0 });
	vertices.push_back({ -side, -side, side, 0, 0 });
	vertices.push_back({ side, -side, side, 0, 0 });
	vertices.push_back({ -side, side, side, 0, 0 });
	vertices.push_back({ side, side, side, 0, 0 });

	m_VertexBuffer.Create(vertices);

	m_IndexBuffer.Create({ 0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4 });

	m_InputLayout.Create<Vertex>("VertexShader_vs");
	
	m_VertexShader.Create("VertexShader_vs");
	m_PixelShader.Create("PixelShader_ps");

	auto ship = AssetManager::GetAsset<ModelSourceAsset>("Assets/Models/Ship.fbx");

	m_ShipMesh = ship->GetMeshAssets().at(0)->GetMesh();

	m_SkyboxLayout.Create<SkyboxVertex>("Skybox_vs");
	m_SkyboxVertexShader.Create("Skybox_vs");
	m_SkyboxShader.Create("Skybox_ps");
	m_SkyboxRasterizer.Create(ECullMode::Front);
	m_SkyboxDepth.Create(EDepthMode::ReadOnly);
}

void Nova::Graphics::Renderer::RenderCube(const Camera& camera)
{
	m_VertexShader.Bind();
	m_PixelShader.Bind();

	m_InputLayout.Bind();
	m_DepthStencilState.Bind();
	m_Rasterizer.Bind();

	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();

	m_TransformBuffer.Data.ProjectionViewMatrix =
		camera.GetViewMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(camera.FovAngle, m_Framework.GetAspectRatio(), camera.NearClipPlane, camera.FarClipPlane);

	m_TransformBuffer.Data.ModelMatrix =
		DirectX::XMMatrixIdentity();

	m_TransformBuffer.ApplyBuffer();
	m_TransformBuffer.Bind();

	DX11::GetContext()->DrawIndexed(m_IndexBuffer.Length(), 0, 0);
}

void Nova::Graphics::Renderer::RenderShip(const Camera& camera)
{
	m_VertexShader.Bind();
	m_PixelShader.Bind();

	m_InputLayout.Bind();
	m_DepthStencilState.Bind();
	m_Rasterizer.Bind();

	m_ShipMesh->Bind();

	m_TransformBuffer.Data.ProjectionViewMatrix =
		camera.GetViewMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(camera.FovAngle, m_Framework.GetAspectRatio(), camera.NearClipPlane, camera.FarClipPlane);

	m_TransformBuffer.Data.ModelMatrix =
		DirectX::XMMatrixIdentity();

	m_TransformBuffer.ApplyBuffer();
	m_TransformBuffer.Bind();

	m_ShipMesh->DrawIndexed();
}

void Nova::Graphics::Renderer::RenderModel(const Transform& transform, const Mesh& mesh, const Material* material, const Camera& camera)
{
	m_VertexShader.Bind();

	m_InputLayout.Bind();
	m_DepthStencilState.Bind();
	m_Rasterizer.Bind();

	mesh.Bind();
	if (material) material->Bind();
	else m_PixelShader.Bind();

	m_TransformBuffer.Data.ProjectionViewMatrix =
		camera.GetViewMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(camera.FovAngle, m_Framework.GetAspectRatio(), camera.NearClipPlane, camera.FarClipPlane);

	m_TransformBuffer.Data.ModelMatrix =
		transform.CalculateMatrix();

	m_TransformBuffer.ApplyBuffer();
	m_TransformBuffer.Bind();

	mesh.DrawIndexed();
}

void Nova::Graphics::Renderer::RenderSkybox(const CubeTexture& skyboxTexture, const Camera& camera)
{
	m_SkyboxVertexShader.Bind();
	m_SkyboxShader.Bind();

	m_SkyboxLayout.Bind();
	m_SkyboxDepth.Bind();
	m_SkyboxRasterizer.Bind();

	m_SkyboxMesh.Bind();

	m_TransformBuffer.Data.ProjectionViewMatrix =
		camera.GetViewMatrix() *
		DirectX::XMMatrixPerspectiveFovLH(camera.FovAngle, m_Framework.GetAspectRatio(), camera.NearClipPlane, camera.FarClipPlane);

	m_TransformBuffer.Data.ModelMatrix =
		DirectX::XMMatrixIdentity();

	m_TransformBuffer.ApplyBuffer();
	m_TransformBuffer.Bind();

	skyboxTexture.Bind();
	m_SkyboxSampler.Bind();

	m_SkyboxMesh.DrawIndexed();
}
