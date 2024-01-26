#include "smolPCH.h"
#include "OpenGLInstance.h"

namespace smol {

	std::unique_ptr<OpenGLEnabler> OpenGLInstance::s_RendererAPI = std::make_unique<OpenGLEnabler>();

}