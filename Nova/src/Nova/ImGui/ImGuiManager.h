#pragma once

namespace Nova
{
	class IImGuiLayer;
}

namespace Nova
{
	class ImGuiManager
	{
	public:
		ImGuiManager(HWND hwnd);
		~ImGuiManager();

		void BeginFrame() const;
		static bool ImGuiInputWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void RenderLayers();
		void EndFrame() const;

		static const IImGuiLayer* AddLayer(std::unique_ptr<IImGuiLayer> layer);
		static bool RemoveLayer(const IImGuiLayer* layer);

	private:
		static ImGuiManager& Get();

		std::vector<std::unique_ptr<IImGuiLayer>> m_Layers;

		static ImGuiManager* m_Instance;
	};
}
