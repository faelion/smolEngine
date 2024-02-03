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
		m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
	}



	smol::Renderer2D::ResetStats();
	m_FrameBuffer->Bind();
	smol::OpenGLInstance::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	m_FrameBuffer->Clear();

	m_FrameBuffer->ClearBuffer(-1);


	m_EditorCamera.OnUpdate();


	//Rendering debugging
	smol::Renderer2D::BeginScene(m_EditorCamera);



	smol::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0, 0, 1, 1));
	smol::Renderer2D::DrawLine({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });



	smol::Renderer2D::EndScene();

	m_FrameBuffer->Unbind();
}

void Viewport::OnEvent(SDL_Event& e)
{
	smol::App::Get().GetImguiLayer().GetEnabler()->ProcessSDL_Events(e);
	m_EditorCamera.OnEvent(e);
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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}


	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

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

	style.WindowMinSize.x = minWinSizeX;
	ImGui::End();

	ImGui::Begin("Stats");

#if 0
	std::string m_Name = "None";
	if (m_HoveredEntity)
		m_Name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
	ImGui::Text("Hovered Entity: %s", m_Name.c_str());
#endif

	auto stats = smol::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
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

	ImGui::Texture((ImTextureID)m_FrameBuffer->getColorBufferTexture(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	
	ImGui::End();
	ImGui::PopStyleVar();
}

void Viewport::CleanUp()
{
}
