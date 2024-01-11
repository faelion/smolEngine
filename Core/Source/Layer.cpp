#include "smolPCH.h"
#include "Layer.h"

namespace smol
{
	Layer::Layer(const std::string& name) : m_DebugName(name) {}

	Layer::~Layer(){}

	void Layer::AddModule(LayerModule* m)
	{
		m_modules.emplace(m_modules.begin() + m_ModuleInsertIndex, m);
		m_ModuleInsertIndex++;
	}

	void Layer::OnAttach()
	{
		for (auto const& m : m_modules)
		{
			if (!m->m_active) continue;
			m->Init();
		}

		for (auto const& m : m_modules)
		{
			if (!m->m_active) continue;
			m->Start();
		}
	}

	void Layer::OnDetach()
	{
		for (auto const& m : m_modules)
		{
			if (!m->m_active) continue;
			m->CleanUp();
		}
	}

	void Layer::OnUpdate()
	{
		for (auto const& m : m_modules)
		{
			if (!m->m_active) continue;
			m->Update();
		}
	}

	void Layer::OnEvent(SDL_Event& e)
	{
		for (auto it = m_modules.end(); it != m_modules.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.user.code)
				break;
		}
	}

	void Layer::OnImGuiRender()
	{
		for (auto const& m : m_modules)
		{
			if (!m->m_active) continue;
			m->OnRenderUI();
		}
	}
	void Layer::CleanUp()
	{
		for (auto const& m : m_modules)
		{
			m->CleanUp();
		}
	}
}