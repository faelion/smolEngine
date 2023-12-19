#pragma once

#include "Layer.h"
#include "Modules/Window.h"

namespace smol {
	class CoreLayer : public Layer
	{
	public:
		CoreLayer();
		~CoreLayer();

	private:
		//Modules
		std::unique_ptr<Window> win;

	};

}
