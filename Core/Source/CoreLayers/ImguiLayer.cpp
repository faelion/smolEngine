#include "CoreLayers/ImguiLayer.h"
#include "Application.h"

namespace smol {

	ImguiLayer::ImguiLayer()
		: Layer("ImguiLayer")
	{
		imgui = std::make_unique<ImguiEnabler>(true);

		//The order matters
		AddModule(imgui.get());
	}

	ImguiLayer::~ImguiLayer(){}

	void ImguiLayer::Begin()
	{
		imgui->BeginUI();
	}

	void ImguiLayer::End()
	{
		imgui->EndUI();
	}

}