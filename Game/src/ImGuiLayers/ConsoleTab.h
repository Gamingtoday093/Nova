#pragma once
#include <Nova/ImGui/IImGuiLayer.hpp>

class ConsoleTab : public Nova::IImGuiLayer
{
public:
	void Render() override;
};
