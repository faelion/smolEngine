#include "Viewport.h"
#include "imgui.h"

Viewport::Viewport(bool startEnabled) : LayerModule(startEnabled) {}

Viewport::~Viewport() {}

void Viewport::Init()
{
	m_FrameBuffer = std::make_shared<smol::FrameBuffer>(1280, 720, true);

	m_EditorCamera = smol::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
	smol::Renderer2D::SetLineWidth(4.0f);
}

void Viewport::Update()
{
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(m_FrameBuffer->getWidth() != m_ViewportSize.x || m_FrameBuffer->getHeight() != m_ViewportSize.y))
	{
		m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}



	m_FrameBuffer->Bind();
	smol::OpenGLInstance::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	m_FrameBuffer->Clear();

	m_EditorCamera.OnUpdate();

	//Rendering debugging
	smol::Renderer2D::BeginScene(m_EditorCamera);



	smol::Renderer2D::DrawRect({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f});



	smol::Renderer2D::EndScene();

	m_FrameBuffer->Unbind();
}

void Viewport::OnEvent(SDL_Event& e)
{
	smol::App::Get().GetImguiLayer().GetEnabler()->ProcessSDL_Events(e);
}

void Viewport::OnRenderUI()
{
	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}


	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}


	ImGui::Begin("Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	uint64_t textureID = m_FrameBuffer->getColorBufferTexture();
	ImGui::Texture(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	
	ImGui::End();
}

void Viewport::CleanUp()
{
}
