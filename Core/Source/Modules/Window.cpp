#include "smolPCH.h"
#include "Window.h"
#include "GL/glew.h"
#include "GL/gl.h"

namespace smol {

	static bool s_SDLInitialized = false;

	Window::Window(bool startEnabled) : LayerModule(startEnabled)
	{
		m_Window = NULL;
		m_ScreenSurface = NULL;
		m_Data.Width = WINDOW_WIDTH;
		m_Data.Height = WINDOW_HEIGHT;
	}

	Window::~Window()
	{
	}

	void Window::Init()
	{
		SMOL_CORE_INFO("Creating window {0} {1} {2}", WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			SMOL_CORE_ASSERT("Could not initialize SDL!");
		else
		{
			// Create window
			s_SDLInitialized = true;
			SDL_version compiled;
			SDL_VERSION(&compiled);
			SMOL_CORE_INFO("SDL Compiled with {0}.{1}.{2}", std::to_string(compiled.major), std::to_string(compiled.minor), std::to_string(compiled.patch));

			SDL_version linked;
			SDL_GetVersion(&linked);
			SMOL_CORE_INFO("SDL Linked with {0}.{1}.{2}", std::to_string(linked.major), std::to_string(linked.minor), std::to_string(linked.patch));


			Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

			//Use OpenGL 2.1
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

			if (FULLSCREEN) flags |= SDL_WINDOW_FULLSCREEN;
			if (BORDERLESS) flags |= SDL_WINDOW_BORDERLESS;
			if (RESIZABLE) flags |= SDL_WINDOW_RESIZABLE;
			if (FULLSCREEN_WINDOW) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

			m_Fullscreen = FULLSCREEN_WINDOW;


			//// setup SDL window
			//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
			//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			//// GL 3.1 + GLSL 130
			//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
			//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

			m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Data.Width, m_Data.Height, flags);

			if (m_Window == NULL)
				{SMOL_CORE_WARN("Window is NULL!");}
			else
				m_ScreenSurface = SDL_GetWindowSurface(m_Window);
		}

		GLenum status = glewInit();
		SMOL_CORE_INFO("Using Glew {0}", glewGetString(GLEW_VERSION));
		SMOL_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		SMOL_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		SMOL_CORE_INFO("OpenGL version supported {0}", glGetString(GL_VERSION));
		SMOL_CORE_INFO("GLSL: {0}\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		SMOL_CORE_ASSERT(!status, "Failed to initialize Glew!");
		SetVSync(VSYNC);
		SetResizable(RESIZABLE);
	}
	void Window::CleanUp()
	{
		SMOL_CORE_INFO("Destroying SDL window and quitting all SDL systems");

		// Destroy window
		if (m_Window != NULL)
		{
			SDL_DestroyWindow(m_Window);
		}

		// Quit SDL subsystems
		SDL_Quit();
	}
	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);

		m_Data.VSync = enabled;
	}
	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}
	void Window::ToggleFullScreen()
	{
		Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
		bool IsFullscreen = SDL_GetWindowFlags(m_Window) & FullscreenFlag;
		SDL_SetWindowFullscreen(m_Window, IsFullscreen ? 0 : FullscreenFlag);

		SMOL_CORE_INFO("Window fullscreen changed {0}", IsFullscreen);
		m_Fullscreen = IsFullscreen;
	}
	void Window::SetResizable(bool enabled)
	{
		/*glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, enabled);*/
		m_Resizable = enabled;
	}
	void Window::SetTitle(const char* title)
	{
		SDL_SetWindowTitle(m_Window, title);
	}
}