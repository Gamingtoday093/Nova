#include "novapch.h"
#include "Component/Components.h"
#include "Component/TestScript.h"
#include "Entity.h"
#include "Nova/Assets/AssetFormats/MeshImportAsset.h"
#include "Nova/Assets/AssetManager.h"
#include "Nova/Tools/Stopwatch.h"
#include "Scene.h"

Nova::Scene::Scene() : m_FreeLookCamera({ 0, 1.5f, -3 }, { 0, 0 }, 8.f, 0.8f)
{
	Entity entity = CreateEntity("Cool Entity");
	entity.AddComponent<TestScript>();
}

Nova::Entity Nova::Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), this };
	entity.AddComponent<TagComponent>(name);

	return entity;
}

const Nova::Camera& Nova::Scene::GetCamera() const
{
	return m_FreeLookCamera;
}
