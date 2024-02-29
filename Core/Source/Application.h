#pragma once
#include "smolPCH.h"
#include "LayerStack.h"
#include "CoreLayers/ImguiLayer.h"
#include "CoreLayers/CoreLayer.h"
#include "Modules/Renderer.h"

namespace smol
{
	namespace Timer
	{
		using namespace std;
		using namespace chrono;

		static const unsigned int targetFPS = 60;
		static const auto dt = 1.0s / targetFPS;
	}

	class App
	{
	public:
		App(int argc, char** argv);
		virtual ~App();

		void Run();

		void OnEvent(SDL_Event& e);

		void PrepareUpdate();

		void Update();

		void FinishUpdate();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);


		inline int GetArgC() { return m_argC; }
		inline const char* GetArgv(int index) { return m_argv[index].c_str(); }

		inline static App& Get() { return *s_Instance; }
		inline CoreLayer& GetCore() { return *m_CoreLayer; }
		inline Window& GetWindow() { return m_CoreLayer->GetWindow(); }
		inline SDL_Window* GetNativeWindow() { return m_CoreLayer->GetWindow().Get(); }
		inline ImguiLayer& GetImguiLayer() { return *m_ImguiLayer; }
		inline Input& GetInput() { return  m_CoreLayer->GetInput(); }


		inline ImGuiContext* GetImGuiContext() { return m_ImguiLayer->GetContext(); }

	private:
		bool OnWindowClose(SDL_Event& e);

		void RegisterEvents();

		void CleanUp();

		LayerStack m_LayerStack;
		ImguiLayer* m_ImguiLayer;
		CoreLayer* m_CoreLayer;

		Timer::steady_clock::time_point m_frameStart;
		Timer::steady_clock::time_point m_frameEnd;
		Timer::nanoseconds frameDuration;
		//std::vector<float> fpsHistory;
		float m_averageFps = 0.0f;
		bool m_frcap = FRAMECAP;

		bool m_Running = true;

		int m_argC = 0;
		std::vector<std::string> m_argv;

	private:
		static App* s_Instance;
	};

	// To be defined in a client
	App* CreateApplication(int argc, char** argv);
}