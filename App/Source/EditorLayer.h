#pragma once

#include <smol.h>

//Modules
#include "Modules/Viewport.h"

class EditorLayer : public smol::Layer
{
public:
	EditorLayer();
	~EditorLayer();

	void Start() override;
	void Update() override;
	void CleanUp() override;

private:
	std::unique_ptr<Viewport> m_Viewport;
};

