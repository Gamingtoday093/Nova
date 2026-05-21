#include "novapch.h"
#include "WindowApplication.h"
#include "Nova/Input/Input.h"
#include "Nova/Time/Time.h"
#include "Nova/Graphics/DX11.h"
#include "Nova/Graphics/Renderer/Renderer.h"
#include "Nova/Scene/Scene.h"
#include "Nova/Assets/AssetManager.h"
#include "Nova/Assets/AssetFormats/SkyboxAsset.h"

Nova::WindowApplication::WindowApplication(const ApplicationStartupInfo& startupInfo) : IApplication(startupInfo)
{
	Log::Initialize(m_StartupInfo.Name);
	m_Time = std::make_unique<Time>();
	m_Input = std::make_unique<Input>();

	WindowStartupInfo windowStartupInfo
	{
		.Title = m_StartupInfo.Name
	};

	m_Window = std::make_unique<Window>(windowStartupInfo);
	m_Window->SetWindowEventCallback([this](WindowEvent& e) { OnWindowEvent(e); });

	m_AssetManager = std::make_unique<AssetManager>();

	Graphics::GraphicsContextParameters contextParameters(m_Window->GetHWND());
	m_IsMinimized = contextParameters.Width == 0 || contextParameters.Height == 0;
	m_Framework = std::make_unique<Graphics::DX11>(contextParameters);
	m_Renderer = std::make_unique<Graphics::Renderer>(*m_Framework);

	m_Scene = std::make_unique<Scene>();
}

Nova::WindowApplication::~WindowApplication() = default;

void Nova::WindowApplication::OnWindowEvent(WindowEvent& windowEvent)
{
	switch (windowEvent.GetEventType())
	{
		case EWindowEvent::WindowResize:
			WindowResizeEvent& resizeEvent = static_cast<WindowResizeEvent&>(windowEvent);
			m_IsMinimized = resizeEvent.Width == 0 || resizeEvent.Height == 0;
			m_Framework->Resize(resizeEvent.Width, resizeEvent.Height);

			// Rerender at new Resolution
			BeginFrame();
			RenderFrame();
			EndFrame();
			
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

		RenderFrame();
		m_Scene->Update();
		m_Scene->m_FreeLookCamera.Update();
		OnUpdate();

		// ----------
		EndFrame();
	}
	OnShutdown();
}

void Nova::WindowApplication::RenderFrame()
{
	//m_Renderer->RenderCube(m_Scene->m_FreeLookCamera);
	//m_Renderer->RenderShip(m_Scene->GetCamera());
	m_Scene->RenderEntities(*m_Renderer);

	auto skyboxTexture = AssetManager::GetAsset<SkyboxAsset>("Assets/Textures/Skybox");
	m_Renderer->RenderSkybox(skyboxTexture->GetSkyboxTexture(), m_Scene->GetCamera());
}

void Nova::WindowApplication::BeginFrame()
{
	m_Time->BeginFrame();
	m_Input->BeginFrame();
	if (m_IsMinimized) return;
	static constexpr float clearColor[4] = { 0.16f, 0.16f, 0.16f, 0.16f };
	m_Framework->BeginFrame(clearColor);
}

void Nova::WindowApplication::EndFrame()
{
	if (m_IsMinimized) return;
	m_Framework->EndFrame();
}
