#pragma once
#include <Nova/ImGui/IImGuiLayer.hpp>

struct EditorContext;

class HierarchyTab : public Nova::IImGuiLayer
{
public:
	HierarchyTab(EditorContext& context);

	void Render() override;

private:
	EditorContext& m_Context;
};
