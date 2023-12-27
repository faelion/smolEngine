#pragma once

#include "SDL_events.h"

namespace smol {
	class LayerModule
	{
	public:

		LayerModule(bool startEnabled = false) : m_active(startEnabled)
		{}
		// Called on the layer attachment
		virtual void Init(){}

		// Called after init
		virtual void Start(){}

		// Called each loop iteration
		virtual void PreUpdate(){}

		// Called each loop iteration
		virtual void Update(){}

		// Called each loop iteration
		virtual void PostUpdate(){}

		// Called each loop iteration
		virtual void OnRenderUI(){}

		// Called before quitting
		virtual void CleanUp(){}

		// Called each loop iteration first
		virtual void OnEvent(SDL_Event& e) {}

		void Enable() {
			if (!m_active) {
				m_active = true;
				Start();
			}
		}

		void Disable() {
			if (m_active) {
				m_active = false;
				CleanUp();
			}
		}

		inline bool IsEnabled() const { return m_active; }

	public:
		bool m_active = 0;

	};

}
