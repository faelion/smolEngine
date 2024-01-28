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
		inline const std::string& GetName() const { return m_DebugName; }

		virtual void Start() {}
		virtual void Update() {}
		virtual void CleanUp() {}

	protected:
		std::string m_DebugName;

		std::vector<LayerModule*> m_modules;
		unsigned int m_ModuleInsertIndex = 0;
	};
}

