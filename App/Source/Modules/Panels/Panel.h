#pragma once

#include <string>
#include "LayerModule.h"

class EditorLayer;

class Panel : public smol::LayerModule
{
public:

	Panel(const char* name, EditorLayer* instance) : m_Name(name), m_Instance(instance) {}
	virtual ~Panel() = default;

	void Enable() //Overloaded
	{
		if (!m_active) {
			m_active = true;
		}
	}

	void Disable() //Overloaded
	{
		if (m_active) {
			m_active = false;
		}
	}

	virtual void Draw() = 0;
	const char* GetName() const { return m_Name.c_str(); }
public:
protected:
	std::string m_Name;
	EditorLayer* m_Instance;
};