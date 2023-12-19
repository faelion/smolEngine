#pragma once

#include "Layer.h"
#include "Modules/Window.h"

namespace smol {
	class CoreLayer : public Layer
	{
	public:
		CoreLayer();
		~CoreLayer();

		inline Window& GetWindow() { return *m_Window; }

	private:
		//Modules
		std::unique_ptr<Window> m_Window;
	};

}
