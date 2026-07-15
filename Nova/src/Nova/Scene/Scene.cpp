#include "novapch.h"
#include "Component/Components.h"
#include "Component/TestScript.h"
#include "Entity.h"
#include "Nova/Assets/AssetFormats/MeshAsset.h"
#include "Nova/Assets/AssetManager.h"
#include "Nova/Tools/Stopwatch.h"
#include "Scene.h"
#include "Nova/Graphics/Renderer/Renderer.h"
#include "Nova/Assets/AssetFormats/ModelSourceAsset.h"
#include "Nova/Assets/AssetFormats/Texture2DAsset.h"
#include "Nova/Assets/AssetFormats/MaterialAsset.h"
#include "Nova/Graphics/Resources/Material/MaterialFormats/StandardMaterial.h"
#include "Nova/Assets/AssetFormats/ShaderAsset.h"
#include "Nova/Graphics/Renderer/GizmoRenderer.h"

static void ReleaseScriptCollection(entt::registry& registry, entt::entity entity)
{
	auto& scriptCollection = registry.get<Nova::Scripts::ScriptCollectionComponent>(entity);
	scriptCollection.Release();
}

Nova::Scene::Scene() : m_FreeLookCamera({ 25, 2.5f, -6.f }, { -1, 0 }, 10.f, 2.2f)
{
	// Destructor doesn't work with EnTT Groups
	m_Registry.on_destroy<Scripts::ScriptCollectionComponent>().connect<&ReleaseScriptCollection>();

	auto shipTexture = AssetManager::GetAsset<Texture2DAsset>("Assets/Textures/ShipTextureB.png")->GetTexture();
	auto shader = AssetManager::GetAsset<ShaderAsset>("Assets/CompiledShaders/PixelShader_ps.cso");
	auto shipMaterial = AssetManager::CreateMaterialAsset<Graphics::StandardMaterial>("ShipMaterial")->GetMaterial<Graphics::StandardMaterial>();
	shipMaterial->AlbedoTexture = shipTexture;

	auto mesh = AssetManager::GetAsset<ModelSourceAsset>("Assets/Models/Ship.fbx")->GetMeshAssets().at(0)->GetMesh();

	Entity entity = CreateEntity("Cool Entity");
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<MeshRendererComponent>(mesh, std::make_shared<Graphics::Material>(shader->GetShader()));

	Entity entity2 = CreateEntity("Cool Entity");
	auto& transform2 = entity2.AddComponent<TransformComponent>();
	transform2.Transform.Position.x = 20.f;
	transform2.Transform.Scale = { 0.5f, 0.5f, 0.5f };
	entity2.AddComponent<MeshRendererComponent>(mesh, shipMaterial);
	//entity2.AddComponent<TestScript>();
}

Nova::Entity Nova::Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), this };
	entity.AddComponent<TagComponent>(name);

	return entity;
}

std::vector<Nova::Entity> Nova::Scene::GetAllEntities()
{
	auto view = m_Registry.view<entt::entity>();
	std::vector<Entity> entities;
	entities.reserve(view.size_hint());
	
	for (auto entity : view)
		entities.emplace_back(entity, this);

	return entities;
}

void Nova::Scene::Update()
{
	auto scripts = m_Registry.group<Scripts::ScriptCollectionComponent>();
	for (auto entity : scripts)
	{
		auto& scriptCollection = scripts.get<Scripts::ScriptCollectionComponent>(entity);
		scriptCollection.Update();
	}
}

void Nova::Scene::RenderEntities(Graphics::Renderer& renderer)
{
	auto renderers = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
	for (auto entity : renderers)
	{
		auto pair = renderers.get<TransformComponent, MeshRendererComponent>(entity);
		if (!std::get<1>(pair).Mesh) continue;
		renderer.RenderModel(std::get<0>(pair).Transform, *std::get<1>(pair).Mesh, std::get<1>(pair).Material.get(), GetCamera());
	}
}

const Nova::Camera& Nova::Scene::GetCamera() const
{
	return m_FreeLookCamera;
}
