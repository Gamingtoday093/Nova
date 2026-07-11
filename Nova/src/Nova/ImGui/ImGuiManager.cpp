#include "novapch.h"
#include "ImGuiManager.h"
#include "IImGuiLayer.hpp"
#include <ImGui/ImGui.h>
#include <ImGuizmo/ImGuizmo.h>
#include "Nova/Graphics/DX11.h"

Nova::ImGuiManager* Nova::ImGuiManager::m_Instance;

Nova::ImGuiManager::ImGuiManager(HWND hwnd)
{
	NOVA_ASSERT(!m_Instance, "Creating multiple ImGuiManagers is not allowed");
	m_Instance = this;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport / Platform Windows
	io.Fonts->AddFontDefault();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Graphics::DX11::GetDevice(), Graphics::DX11::GetContext());
}

Nova::ImGuiManager::~ImGuiManager()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	ImGui::DestroyContext();

	m_Instance = nullptr;
}

void Nova::ImGuiManager::BeginFrame() const
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	for (auto& layer : m_Layers)
		layer->BeginFrame();
}

bool Nova::ImGuiManager::ImGuiInputWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
}

void Nova::ImGuiManager::RenderLayers()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None); // ImGuiDockNodeFlags_PassthruCentralNode

	for (auto& layer : m_Layers)
		layer->Render();
}

void Nova::ImGuiManager::EndFrame() const
{
	for (auto& layer : m_Layers)
		layer->EndFrame();

	ImGui::Render();
	
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

const Nova::IImGuiLayer* Nova::ImGuiManager::AddLayer(std::unique_ptr<IImGuiLayer> layer)
{
	return Get().m_Layers.emplace_back(std::move(layer)).get();
}

bool Nova::ImGuiManager::RemoveLayer(const IImGuiLayer* layer)
{
	auto& layers = Get().m_Layers;
	for (int32_t i = int32_t(layers.size()) - 1; i >= 0; i--)
	{
		if (layers[i].get() != layer) continue;
		layers.erase(layers.begin() + i);
		return true;
	}

	return false;
}

Nova::ImGuiManager& Nova::ImGuiManager::Get()
{
	NOVA_ASSERT(m_Instance, "ImGuiManager hasn't been Initialized");
	return *m_Instance;
}
