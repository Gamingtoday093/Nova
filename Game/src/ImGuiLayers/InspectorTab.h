#pragma once
#include <Nova/ImGui/IImGuiLayer.hpp>

struct EditorContext;

class InspectorTab : public Nova::IImGuiLayer
{
public:
	InspectorTab(EditorContext& context);

	void Render() override;

private:
	EditorContext& m_Context;
};
