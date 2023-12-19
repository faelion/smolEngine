#include "CoreLayer.h"

namespace smol {
	CoreLayer::CoreLayer()
		: Layer("CoreLayer")
	{
		m_Window = std::make_unique<Window>(true);

		//The order matters
		AddModule(m_Window.get());

	}
	CoreLayer::~CoreLayer(){}
}