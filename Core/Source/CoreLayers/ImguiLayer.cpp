#include "smolPCH.h"
#include "CoreLayers/ImguiLayer.h"
#include "Application.h"

namespace smol {

	ImguiLayer::ImguiLayer()
		: Layer("ImguiLayer")
	{
		m_ImguiEnabler = std::make_unique<ImguiEnabler>(true);

		//The order matters
		AddModule(m_ImguiEnabler.get());
	}

	ImguiLayer::~ImguiLayer(){}

	void ImguiLayer::Begin()
	{
		m_ImguiEnabler->BeginUI();
	}

	void ImguiLayer::End()
	{
		m_ImguiEnabler->EndUI();
	}

}