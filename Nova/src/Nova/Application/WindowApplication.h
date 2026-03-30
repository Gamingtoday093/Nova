#pragma once
#include "IApplication.hpp"
#include "Nova/Window/Window.h"

namespace Nova::Graphics
{
	class DX11;
}

namespace Nova
{
	class WindowApplication : public IApplication
	{
	public:
		WindowApplication(const ApplicationStartupInfo& startupInfo);

		void Run() override;

		virtual void OnStart() { }
		virtual void OnUpdate() { }
		virtual void OnShutdown() { }

	private:
		void OnWindowEvent(WindowEvent& windowEvent);

		void BeginFrame();
		void EndFrame();

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Graphics::DX11> m_Framework;
	};
}
