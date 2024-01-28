#pragma once

#include "Layer.h"
#include "Modules/Window.h"
#include "Modules/Input.h"

namespace smol {
	class CoreLayer : public Layer
	{
	public:
		CoreLayer();
		~CoreLayer();

		void Start() override { SMOL_CORE_TRACE("Core Layer Started"); }
		void Update() override {}
		void CleanUp() override { SMOL_CORE_TRACE("Core Layer Cleaned Up"); }

		inline Window& GetWindow() { return *m_Window; }
		inline Input& GetInput() { return *m_Input; }

	private:
		//Modules
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Input> m_Input;
	};

}
