#include "novapch.h"
#include "Components.h"

Nova::TagComponent::TagComponent(const std::string& name)
{
	Name = name;
}

Nova::MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Graphics::Mesh> mesh)
{
	Mesh = std::move(mesh);
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
	for (auto& script : m_Scripts)
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

void Nova::Scripts::ScriptCollectionComponent::Update()
{
	for (auto& script : m_Scripts)
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
