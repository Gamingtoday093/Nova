#include "novapch.h"
#include "Component/Components.h"
#include "Component/TestScript.h"
#include "Entity.h"
#include "Nova/Assets/AssetFormats/MeshImportAsset.h"
#include "Nova/Assets/AssetManager.h"
#include "Nova/Tools/Stopwatch.h"
#include "Scene.h"
#include "Nova/Graphics/Renderer/Renderer.h"
#include "Nova/Assets/AssetFormats/ModelSourceAsset.h"

Nova::Scene::Scene() : m_FreeLookCamera({ 0, 1.5f, -3 }, { 0, 0 }, 8.f, 0.8f)
{
	Entity entity = CreateEntity("Cool Entity");
	entity.AddComponent<TransformComponent>();

	auto mesh = AssetManager::GetAsset<ModelSourceAsset>("Assets/Models/Ship.fbx")->GetMeshAssets().at(0)->GetMesh();
	entity.AddComponent<MeshRendererComponent>(mesh);

	Entity entity2 = CreateEntity("Cool Entity");
	auto& transform = entity2.AddComponent<TransformComponent>();
	transform.Transform.Position.x = 20.f;
	transform.Transform.Scale = { 0.5f, 0.5f, 0.5f };
	entity2.AddComponent<MeshRendererComponent>(mesh);
}

Nova::Entity Nova::Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), this };
	entity.AddComponent<TagComponent>(name);

	return entity;
}

void Nova::Scene::RenderEntities(Graphics::Renderer& renderer)
{
	auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
	for (auto entity : group)
	{
		auto pair = group.get<TransformComponent, MeshRendererComponent>(entity);
		renderer.RenderModel(std::get<0>(pair).Transform, *std::get<1>(pair).Mesh, GetCamera());
	}
}

const Nova::Camera& Nova::Scene::GetCamera() const
{
	return m_FreeLookCamera;
}
