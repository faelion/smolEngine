#pragma once

#include "LayerModule.h"
#include "SDL.h"
#include "Defs.h"
#include "Events/Event.h"

namespace smol {
	class Window : public LayerModule
	{
	private:
		struct WindowData
		{
			std::string Title = "";
			uint32_t Width = 0;
			uint32_t Height = 0;
			bool VSync = false;
			uint32_t Refresh = 0;
		};

		WindowData m_Data;
		WindowData m_OldData;

		bool m_Fullscreen = false;
		bool m_Resizable = false;
	public:
		Window(bool startEnabled);

		virtual ~Window();

		// Called when adding the modules to the Layer
		void Init();

		// Called before quitting
		void CleanUp();

		void OnEvent(SDL_Event& e);

		inline WindowData& GetWindowData() { return m_Data; }

		inline SDL_Window* Get() { return m_Window; }

		inline uint32_t GetWidth() const { return m_Data.Width; }
		inline uint32_t GetHeight() const { return m_Data.Height; }
		inline bool GetFullScreen() const { return m_Fullscreen; }
		inline bool GetResizable() const { return m_Resizable; }

		//Window attributes 
		void SetVSync(bool enabled);
		bool IsVSync() const;
		void ToggleFullScreen();
		void SetResizable(bool enabled);
		void SetTitle(const char* title);

	private:
		SDL_Window* m_Window;

		SDL_Surface* m_ScreenSurface;
	};

}

