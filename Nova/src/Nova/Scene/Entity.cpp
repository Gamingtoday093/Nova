#include "novapch.h"
#include "Entity.h"
#include "Component/Components.h"

Nova::Entity::Entity(entt::entity handle, Scene* scene) : m_Handle(handle), m_Scene(scene) { }

Nova::ScriptBase& Nova::Entity::AddScript(Scripts::ScriptCollectionComponent& scriptCollection, Scripts::ScriptID scriptID, ScriptBase* script)
{
    return scriptCollection.AddScript(*this, scriptID, script);
}

Nova::ScriptBase* Nova::Entity::TryGetScript(const Scripts::ScriptCollectionComponent& scriptCollection, Scripts::ScriptID scriptID) const
{
    return scriptCollection.TryGetScript(scriptID);
}

bool Nova::Entity::RemoveScript(Scripts::ScriptCollectionComponent& scriptCollection, Scripts::ScriptID scriptID)
{
    return scriptCollection.RemoveScript(*this, scriptID);
}

entt::entity Nova::Entity::GetEntityHandle() const
{
    return m_Handle;
}

Nova::Scene* Nova::Entity::GetScene() const
{
    return m_Scene;
}

bool Nova::Entity::operator==(const Entity& entity) const
{
    return IsValid() && m_Handle == entity.m_Handle && m_Scene == entity.m_Scene;
}

bool Nova::Entity::IsValid() const
{
    return m_Scene && m_Scene->m_Registry.valid(m_Handle);
}

Nova::Entity::operator bool() const
{
    return IsValid();
}
