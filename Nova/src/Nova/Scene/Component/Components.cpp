#include "novapch.h"
#include "Components.h"
#include "Nova/Graphics/Resources/Mesh.h"
#include "Nova/Graphics/Resources/Material/Material.h"

Nova::TagComponent::TagComponent(const std::string& name)
{
	Name = name;
}

Nova::MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Graphics::Mesh> mesh)
{
	Mesh = std::move(mesh);
}

Nova::MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Graphics::Mesh> mesh, std::shared_ptr<Graphics::Material> material)
{
	Mesh = std::move(mesh);
	Material = std::move(material);
}

Nova::Graphics::Bounds Nova::MeshRendererComponent::GetBounds(const Graphics::Transform& transform) const
{
	if (!Mesh) return Graphics::Bounds(transform.Position, { 0, 0, 0 });

	Graphics::Bounds bounds = Mesh->GetBounds();
	XMVECTOR center = DirectX::XMLoadFloat3(&bounds.Center);
	XMVECTOR extents = DirectX::XMLoadFloat3(&bounds.Extents);

	XMMATRIX matrix = transform.CalculateMatrix();
	center = DirectX::XMVector3Transform(center, matrix);
	extents = DirectX::XMVector3TransformNormal(extents, matrix);

	DirectX::XMStoreFloat3(&bounds.Center, center);
	DirectX::XMStoreFloat3(&bounds.Extents, extents);
	return bounds;
}

void Nova::Scripts::ScriptCollectionComponent::Release()
{
	for (auto* script : m_Scripts)
		delete script;

	m_Scripts.clear();
}

Nova::ScriptBase& Nova::Scripts::ScriptCollectionComponent::AddScript(Entity& entity, Scripts::ScriptID scriptID, ScriptBase* script)
{
	m_Scripts.push_back(script);

	script->m_Entity = entity;
	script->m_ScriptID = scriptID;
	script->Awake();
	
	return *script;
}

bool Nova::Scripts::ScriptCollectionComponent::HasScript(Scripts::ScriptID scriptID) const
{
	return TryGetScript(scriptID) != nullptr;
}

Nova::ScriptBase* Nova::Scripts::ScriptCollectionComponent::TryGetScript(Scripts::ScriptID scriptID) const
{
	for (auto* script : m_Scripts)
		if (script->m_ScriptID == scriptID) return script;

	return nullptr;
}

bool Nova::Scripts::ScriptCollectionComponent::RemoveScript(Entity& entity, Scripts::ScriptID scriptID)
{
	for (size_t i = 0; i < m_Scripts.size(); i++)
	{
		if (m_Scripts[i]->m_ScriptID == scriptID)
		{
			delete m_Scripts[i];
			m_Scripts.erase(m_Scripts.begin() + i);
			if (m_Scripts.empty())
				entity.RemoveComponent<ScriptCollectionComponent>();

			return true;
		}
	}

	return false;
}

std::vector<Nova::ScriptBase*>::iterator Nova::Scripts::ScriptCollectionComponent::begin()
{
	return m_Scripts.begin();
}

std::vector<Nova::ScriptBase*>::const_iterator Nova::Scripts::ScriptCollectionComponent::begin() const
{
	return m_Scripts.begin();
}

std::vector<Nova::ScriptBase*>::iterator Nova::Scripts::ScriptCollectionComponent::end()
{
	return m_Scripts.end();
}

std::vector<Nova::ScriptBase*>::const_iterator Nova::Scripts::ScriptCollectionComponent::end() const
{
	return m_Scripts.end();
}

void Nova::Scripts::ScriptCollectionComponent::Update()
{
	for (auto* script : m_Scripts)
	{
		if (!script->m_IsEnabled) continue;

		if (!script->m_HasStarted)
		{
			script->m_HasStarted = true;
			script->Start();
		}
		script->Update();
	}
}
