#pragma once

#include "VertexArray.h"
#include "glm/glm.hpp"

namespace smol {

	class OpenGLEnabler
	{
	public:
		void Init();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void SetClearColor(const glm::vec4& color);
		void Clear();

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount);
		
		void SetLineWidth(float width);
	};


}
