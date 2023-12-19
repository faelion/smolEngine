#pragma once

#include "LayerModule.h"
#include "Imgui.h"

namespace smol {
	class ImguiEnabler : public LayerModule
	{
	public:

		ImguiEnabler(bool startEnabled);

		virtual ~ImguiEnabler();

		// Called when adding the modules to the Layer
		void Init();

		// Called before quitting
		void CleanUp();

		void BeginUI();
		void EndUI();

		inline ImGuiContext* GetImguiContext() { return m_Ctx; }
	private:
		void SetDarkTheme();
		ImGuiContext* m_Ctx = nullptr;
	};
}
