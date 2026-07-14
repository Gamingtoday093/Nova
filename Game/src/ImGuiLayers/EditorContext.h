#pragma once
#include <Nova/Scene/Entity.h>

struct EditorContext
{
	Nova::Scene* Scene = nullptr;
	Nova::Entity SelectedEntity = Nova::Entity::Invalid;
};