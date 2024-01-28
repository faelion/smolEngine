#include "smolPCH.h"
#include "Application.h"

const size_t TYPE_COUNT = __COUNTER__;

namespace smol {

#define BIND_EVENT_FN(x) std::bind(&App::x, this, std::placeholders::_1)

	App* App::s_Instance = nullptr;

	App::App(int argc, char** argv)
	{
		SMOL_CORE_ASSERT(!s_Instance, "App already exists!");

		m_argC = argc;

		for (int i = 0; i < argc; i++) {
			m_argv.push_back(argv[i]);
		}

		s_Instance = this;


		m_CoreLayer = std::make_unique<CoreLayer>();
		PushLayer(m_CoreLayer.get());
		m_ImguiLayer = std::make_unique<ImguiLayer>();
		PushLayer(m_ImguiLayer.get());


		Renderer::Init();
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
		CleanUp();
	}

	void App::PrepareUpdate()
	{
		m_frameStart = std::chrono::steady_clock::now();
	}

	void App::Update()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			OnEvent(e);
		}

		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

		m_ImguiLayer->Begin();
		{
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
		}
		m_ImguiLayer->End();
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

		//if (fpsHistory.size() > 100) fpsHistory.erase(fpsHistory.begin());

		// Shows the time measurements in the window title
		static char title[256];
		sprintf_s(title, 256, "smolEngine | FPS: %.2f, Av.FPS: %.2f, Last-frame MS: %.5f, frame cap: %s"/*, vsync: % s*/,
			lastFPS, m_averageFps, frameDuration * 1000000, m_frcap ? "on" : "off"/*, app->render->vsync ? "on" : "off"*/);

		GetWindow().SetTitle(title);
	}

	void App::OnEvent(SDL_Event& e)
	{
		EventDispatcher dispatcher(e);
		if(e.type == SDL_QUIT)
		dispatcher.Dispatch<SDL_QUIT>(BIND_EVENT_FN(OnWindowClose));
		/*dispatcher.Dispatch<OnSaveEvent>({ &App::OnSave, this });
		dispatcher.Dispatch<OnLoadEvent>({ &App::OnLoad, this });*/

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.user.code)
				break;
		}

		//Check if event is custom
		if (isCustomEvent(e))
		{
			RELEASE_EVENT(e);
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


	bool App::OnWindowClose(SDL_Event& e)
	{
		m_Running = false;
		return true;
	}

	void App::RegisterEvents()
	{
		const int max = static_cast<int>(EventType::NUM_CUSTOM_EVENTS);
		Uint32 customEventTypes[max];

		Uint32 customEventStart = SDL_RegisterEvents(max);
		if (customEventStart != ((Uint32)-1)) {
			for (int i = 0; i < max; i++) {
				customEventTypes[i] = customEventStart + i;
			}
			// Los eventos ahora están registrados y listos para ser usados
		}
		else {
			// Manejar el error si no se pudieron registrar los eventos
			SMOL_CORE_ERROR("Events couldn't register!");
		}
	}

	void App::CleanUp()
	{
		for (Layer* layer : m_LayerStack)
			layer->CleanUp();
	}

}