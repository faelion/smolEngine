#include "EditorLayer.h"

EditorLayer::EditorLayer()
	: Layer("EditorLayer")
{
	m_Viewport = std::make_unique<Viewport>(true);
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::Start()
{
	SMOL_CORE_TRACE("Editor Layer Started");


	//The order matters
	AddModule(m_Viewport.get());
}

void EditorLayer::Update()
{
}

void EditorLayer::CleanUp()
{
	SMOL_CORE_TRACE("Editor Layer Cleaned Up");
}
