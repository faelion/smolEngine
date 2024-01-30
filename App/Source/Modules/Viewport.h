#pragma once

#include <smol.h>
#include "Render/EditorCamera.h"


enum class SceneState
{
	Edit = 0, 
	Play, 
	Simulate
};

class Viewport : public smol::LayerModule
{
public:
	Viewport(bool startEnabled);
	virtual ~Viewport();

	void Init() override;
	void OnEvent(SDL_Event& e) override;
	void Update() override;
	void OnRenderUI() override;
	void CleanUp() override;

private:
	std::shared_ptr<smol::FrameBuffer> m_FrameBuffer;

	smol::EditorCamera m_EditorCamera;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];

	SceneState m_SceneState = SceneState::Edit;
};
