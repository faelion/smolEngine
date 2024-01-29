#include "smolPCH.h"
#include "CoreLayers/ImguiLayer.h"
#include "Application.h"

namespace smol {

	ImguiLayer::ImguiLayer()
		: Layer("ImguiLayer")
	{
		m_ImguiEnabler = std::make_unique<ImguiEnabler>(true);
	}

	ImguiLayer::~ImguiLayer(){}

	void ImguiLayer::Start()
	{
		SMOL_CORE_TRACE("ImGui Layer Started");

		//The order matters
		AddModule(m_ImguiEnabler.get());
	}

	void ImguiLayer::CleanUp()
	{
		SMOL_CORE_TRACE("ImGui Layer Cleaned Up");
	}

	void ImguiLayer::Begin()
	{
		m_ImguiEnabler->BeginUI();
	}

	void ImguiLayer::End()
	{
		m_ImguiEnabler->EndUI();
	}

}