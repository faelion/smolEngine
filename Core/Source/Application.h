#pragma once
#include "smolPCH.h"
#include "LayerStack.h"
#include "CoreLayers/ImguiLayer.h"
#include "CoreLayers/CoreLayer.h"

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


		inline static App& Get() { return *s_Instance; }

	private:
		LayerStack m_LayerStack;
		std::unique_ptr<ImguiLayer> m_ImguiLayer;
		std::unique_ptr<CoreLayer> m_CoreLayer;

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


//Nota: Esto sirve para tener la cuenta de las clases que hay definidas, que mas tarde ser� �til para en el editor mostrar los componentes de los game objects

#define USE_REFLECTION const size_t TYPE_COUNT = __COUNTER__;

#define REFLECTION_BODY_INL \
	size_t getAppTypeCount() const override { return TYPE_COUNT; } \
	const Type* getAppType(size_t index) const override { return GET_TYPES()->at(index); } \
	const Type* getAppTypeH(size_t hash) const override { \
		const Wiwa::Array<const Type*, TYPE_COUNT>* types = GET_TYPES(); \
		const Type* type = NULL; \
		for (size_t i = 0; i < TYPE_COUNT; i++) { \
			if (types->at(i)->hash == hash) { \
				type = types->at(i); \
				break; \
			} \
		} \
		return type; \
	}