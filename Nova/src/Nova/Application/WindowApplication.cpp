#include "pch.h"
#include "WindowApplication.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Renderer/Renderer.h"

Nova::WindowApplication::WindowApplication(const ApplicationStartupInfo& startupInfo) : IApplication(startupInfo)
{
	Log::Initialize(m_StartupInfo.Name);

	WindowStartupInfo windowStartupInfo
	{
		.Title = m_StartupInfo.Name
	};

	m_Window = std::make_unique<Window>(windowStartupInfo);
	m_Window->SetWindowEventCallback([this](WindowEvent& e) { OnWindowEvent(e); });

	Graphics::GraphicsContextParameters contextParameters(m_Window->GetHWND());
	m_Framework = std::make_unique<Graphics::DX11>(contextParameters);
	m_Renderer = std::make_unique<Graphics::Renderer>(*m_Framework);
}

Nova::WindowApplication::~WindowApplication() = default;

void Nova::WindowApplication::OnWindowEvent(WindowEvent& windowEvent)
{
	switch (windowEvent.GetEventType())
	{
		case EWindowEvent::WindowResize:
			WindowResizeEvent& resizeEvent = static_cast<WindowResizeEvent&>(windowEvent);
			m_Framework->Resize(resizeEvent.Width, resizeEvent.Height);
			break;
	}
}

void Nova::WindowApplication::Run()
{
	OnStart();
	while (m_Window->ProcessMessages())
	{
		BeginFrame();
		// ----------

		OnUpdate();
		
		// ----------
		EndFrame();
	}
	OnShutdown();
}

void Nova::WindowApplication::BeginFrame()
{
	static constexpr float clearColour[4] = { 0.16f, 0.16f, 0.16f, 0.16f };
	m_Framework->BeginFrame(clearColour);
}

void Nova::WindowApplication::EndFrame()
{
	m_Framework->EndFrame();
}
