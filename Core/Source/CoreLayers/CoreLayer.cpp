#include "smolPCH.h"
#include "CoreLayer.h"

namespace smol {
	CoreLayer::CoreLayer()
		: Layer("CoreLayer")
	{
		m_Window = std::make_unique<Window>(true);
		m_Input = std::make_unique<Input>(true);

		//The order matters
		AddModule(m_Window.get());
		AddModule(m_Input.get());

	}
	CoreLayer::~CoreLayer(){}
}