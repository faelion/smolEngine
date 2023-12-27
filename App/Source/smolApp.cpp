#include <smol.h>

#include "EditorLayer.h"

class smolApp : public smol::App
{
public:

	smolApp(int argc, char** argv) : smol::App(argc, argv)
	{
		PushOverlay(new EditorLayer());
	}

	~smolApp()
	{

	}
};

smol::App* smol::CreateApplication(int argc, char** argv)
{
	return new smolApp(argc, argv);
}

int main(int argc, char** argv)
{
	smol::Log::Init();
	SMOL_CORE_WARN("Initialized Log!");

	auto app = smol::CreateApplication(argc, argv);

	app->Run();

	delete app;

	return 0;
}
