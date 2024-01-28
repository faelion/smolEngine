#pragma once

#include <smol.h>
#include <Render/Framebuffer.h>

class EditorLayer : public smol::Layer
{
public:
	EditorLayer();
	~EditorLayer();

	void Start() override;
	void Update() override;
	void CleanUp() override;

private:
};

