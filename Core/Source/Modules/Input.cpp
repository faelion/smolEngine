#include "smolPCH.h"
#include "Application.h"
#include "Input.h"
#include "Defs.h"

#define MAX_KEYS 300
namespace smol {
	Input::Input(bool start_enabled) : LayerModule(start_enabled)
	{
		keyboard = new KEY_STATE[MAX_KEYS];
		memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
		memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
	}

	// Destructor
	Input::~Input()
	{
		RELEASE(keyboard);
	}

	// Called before render is available
	void Input::Init()
	{
		SMOL_CORE_INFO("Init SDL input event system");
		SDL_Init(0);

		if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
		{
			SMOL_CORE_INFO("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		}
	}

	// Called every draw update
	void Input::Update()
	{
		SDL_PumpEvents();

		const Uint8* keys = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < MAX_KEYS; ++i)
		{
			if (keys[i] == 1)
			{
				if (keyboard[i] == KEY_IDLE)
					keyboard[i] = KEY_DOWN;
				else
					keyboard[i] = KEY_REPEAT;
			}
			else
			{
				if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
					keyboard[i] = KEY_UP;
				else
					keyboard[i] = KEY_IDLE;
			}
		}

		Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

		//in case of resizable window
		//mouse_x /= SCREEN_SIZE;
		//mouse_y /= SCREEN_SIZE;
		mouse_z = 0;

		for (int i = 0; i < 5; ++i)
		{
			if (buttons & SDL_BUTTON(i))
			{
				if (mouse_buttons[i] == KEY_IDLE)
					mouse_buttons[i] = KEY_DOWN;
				else
					mouse_buttons[i] = KEY_REPEAT;
			}
			else
			{
				if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
					mouse_buttons[i] = KEY_UP;
				else
					mouse_buttons[i] = KEY_IDLE;
			}
		}

		mouse_x_motion = mouse_y_motion = 0;

		// Update the last mouse position at the end of each frame
		lastMouseX = mouse_x;
		lastMouseY = mouse_y;
	}

	// Called before quitting
	void Input::CleanUp()
	{
		SMOL_CORE_INFO("Quitting SDL input event subsystem");
		SDL_QuitSubSystem(SDL_INIT_EVENTS);
	}
}