#include "Application.h"

const size_t TYPE_COUNT = __COUNTER__;

namespace smol {
	App* App::s_Instance = nullptr;

	App::App(int argc, char** argv)
	{
		m_argC = argc;

		for (int i = 0; i < argc; i++) {
			m_argv.push_back(argv[i]);
		}
	}

	App::~App() = default;

	void App::Run()
	{
		while (m_Running)
		{
			PrepareUpdate();
			Update();
			FinishUpdate();
		}
	}

	void App::PrepareUpdate()
	{
		m_frameStart = std::chrono::steady_clock::now();
	}

	void App::Update()
	{
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

		m_ImguiLayer->Begin();
		{
			//TODO: Optick On ImGuiRender call
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
		}
		m_ImguiLayer->End();

		m_Window->OnUpdate();
	}

	void App::FinishUpdate()
	{
		m_frameEnd = std::chrono::steady_clock::now();
		frameDuration = m_frameEnd - m_frameStart;

		if (frameDuration < Timer::dt && m_frcap)
			std::this_thread::sleep_for(Timer::dt - frameDuration);

		const auto frameEndAfterSleep = std::chrono::steady_clock::now();
		const auto frameDurationAfterSleep = frameEndAfterSleep - m_frameStart;


		float lastFPS = 0.0f;
		lastFPS = 1.0f / (frameDurationAfterSleep.count() * 0.000000001f);


		m_averageFps = (m_averageFps + lastFPS) / 2;
		//fpsHistory.push_back(lastFPS);

		//// Replace oldest data in the history
		//if (fpsHistory.size() > 100) fpsHistory.erase(fpsHistory.begin());

		// Shows the time measurements in the window title
		static char title[256];
		sprintf_s(title, 256, "smolEngine | FPS: %.2f, Av.FPS: %.2f, Last-frame MS: %.5f, frame cap: %s, vsync: %s",
			lastFPS, m_averageFps, frameDuration * 1000000, m_frcap ? "on" : "off"/*, app->render->vsync ? "on" : "off"*/);


		//app->win->SetTitle(title);
	}

	void App::OnEvent(SDL_Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>({ &Application::OnWindowClose, this });
		dispatcher.Dispatch<OnSaveEvent>({ &Application::OnSave, this });
		dispatcher.Dispatch<OnLoadEvent>({ &Application::OnLoad, this });

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void App::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void App::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}