#pragma once

#include "smolPCH.h"
#include "LayerModule.h"
#include "Events/Event.h"

namespace smol {
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		void AddModule(LayerModule* layer);

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(SDL_Event& e);
		void OnImGuiRender();
		void CleanUp();
		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;

		std::vector<LayerModule*> m_modules;
		unsigned int m_ModuleInsertIndex = 0;
	};
}

