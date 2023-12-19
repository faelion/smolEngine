#pragma once

#include "smolPCH.h"
#include "Defs.h"
#include "SDL_events.h"

namespace smol {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		OnLoad, OnSave, OnMaterialChange, OnEntityChange
	};

	enum EventCategory
	{
		None = 0, 
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4),
		EventCategoryEditor			= BIT(5)
	};

#define EVENT_CLASS_TYPE(type) static smol::EventType GetStaticType() { return  smol::EventType::##type; } \
								virtual  smol::EventType GetEventType() const override { return GetStaticType(); } \
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class SDL_Event : public SDL_Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	public:
		bool Handled = false;
	};
	
	class EventDispatcher
	{
		template<typename T>
		using EventFn = Fn::Function<bool,T&>;

	public:
		EventDispatcher(SDL_Event& event) : m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		SDL_Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const SDL_Event& e)
	{
		return os << e.ToString();
	}
}