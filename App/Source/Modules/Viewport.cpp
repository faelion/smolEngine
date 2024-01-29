#include "Viewport.h"
#include <memory>

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
	m_FrameBuffer->Bind();

	smol::OpenGLInstance::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	m_FrameBuffer->Clear();
	m_EditorCamera.OnUpdate();

	//Rendering debugging
	smol::Renderer2D::BeginScene(m_EditorCamera);



	smol::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});



	smol::Renderer2D::EndScene();

	m_FrameBuffer->Unbind();
}

void Viewport::CleanUp()
{
}
