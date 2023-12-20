#pragma once

#include "Event.h"

namespace smol {

	class OnLoadEvent : public CustomEvent
	{
	public:
		OnLoadEvent() = default;

		EVENT_CLASS_TYPE(USER_LOAD)
	};
	class OnSaveEvent : public CustomEvent
	{
	public:
		OnSaveEvent() = default;

		EVENT_CLASS_TYPE(USER_SAVE)
	};
}
