#include "smolPCH.h"
#include "CoreLayer.h"

namespace smol {
	CoreLayer::CoreLayer()
		: Layer("CoreLayer")
	{
		m_Window = std::make_unique<Window>(true);
		m_Input = std::make_unique<Input>(true);
	}
	CoreLayer::~CoreLayer(){}


	void CoreLayer::Start()
	{
		SMOL_CORE_TRACE("Core Layer Started");

		//The order matters
		AddModule(m_Window.get());
		AddModule(m_Input.get());
	}
	void CoreLayer::CleanUp()
	{
		SMOL_CORE_TRACE("Core Layer Cleaned Up");
	}
}