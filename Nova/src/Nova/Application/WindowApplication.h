#pragma once
#include "IApplication.hpp"
#include "Nova/Window/Window.h"

namespace Nova::Graphics
{
	class DX11;
	class Renderer;
	class GizmoRenderer;
}

namespace Nova
{
	class Time;
	class Input;
	class AssetManager;
	class Scene;
	class ImGuiManager;
}

namespace Nova
{
	class WindowApplication : public IApplication
	{
	public:
		WindowApplication(const ApplicationStartupInfo& startupInfo);
		~WindowApplication();

		void Run() override;

	protected:
		virtual void OnStart() { }
		virtual void OnUpdate() { }
		virtual void OnShutdown() { }

		std::unique_ptr<Scene> m_Scene;
		HWND GetHWND() const;

	private:
		void OnWindowEvent(WindowEvent& windowEvent);

		void BeginFrame();
		void RenderFrame();
		void EndFrame();

		bool m_IsMinimized;
		std::unique_ptr<Time> m_Time;
		std::unique_ptr<Input> m_Input;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Graphics::DX11> m_Framework;
		std::unique_ptr<Graphics::Renderer> m_Renderer;
		std::unique_ptr<Graphics::GizmoRenderer> m_GizmoRenderer;
		std::unique_ptr<AssetManager> m_AssetManager;
		std::unique_ptr<ImGuiManager> m_ImGuiManager;
	};
}
