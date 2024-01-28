#include "EditorLayer.h"

EditorLayer::EditorLayer()
	: Layer("EditorLayer")
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::Start()
{
	SMOL_CORE_TRACE("Editor Layer Started");
}

void EditorLayer::Update()
{
}

void EditorLayer::CleanUp()
{
	SMOL_CORE_TRACE("Editor Layer Cleaned Up");
}
