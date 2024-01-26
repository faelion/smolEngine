#pragma once
#include "LayerModule.h"
#include <vector>

#define MAX_MOUSE_BUTTONS 5
#define MAX_BUTTONS 15
#define MAX_KEYS 300

namespace smol {
	enum EventWindow
	{
		WE_QUIT = 0,
		WE_HIDE = 1,
		WE_SHOW = 2,
		WE_COUNT
	};

	enum KEY_STATE
	{
		KEY_IDLE = 0,
		KEY_DOWN,
		KEY_REPEAT,
		KEY_UP
	};

	class Input : public LayerModule
	{
	public:

		Input(bool start_enabled);
		~Input();

		void Init();
		void Update();
		void CleanUp();

		//getters
		bool GetWindowEvent(EventWindow ev)
		{
			return windowEvents[ev];
		}

		inline KEY_STATE GetKey(int id) const
		{
			return keyboard[id];
		}

		inline KEY_STATE GetMouseButton(int id) const
		{
			return mouse_buttons[id];
		}

		int GetMouseX() const
		{
			return mouse_x;
		}

		int GetMouseY() const
		{
			return mouse_y;
		}

		int GetMouseZ() const
		{
			return mouse_z;
		}

		int GetMouseXMotion() const
		{
			return mouse_x_motion;
		}

		int GetMouseYMotion() const
		{
			return mouse_y_motion;
		}

	private:
		bool windowEvents[WE_COUNT];
		KEY_STATE* keyboard;
		KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
		int mouse_x = 0;
		int mouse_y = 0;
		int mouse_z = 0;
		int mouse_x_motion = 0;
		int mouse_y_motion = 0;
		int lastMouseX = 0;
		int lastMouseY = 0;
		bool isRightButtonDown = false;
		bool isLeftButtonDown = false;
		bool isMiddleButtonDown = false;
	};
}