#pragma once

#include "Defs.h"
#include "SDL_events.h"


#define PUSH_CUSTOM_EVENT(eventType) \
    SDL_UserEvent customEvent; \
    SDL_zero(customEvent); \
    customEvent.data1 = eventType; \
    customEvent.type = eventType->GetEventType(); \
    customEvent.user.code = 0; \
    SDL_PushEvent(&customEvent);

#define RELEASE_EVENT(e) delete e.user.data1


namespace smol {

	enum class EventType // Custom Event Type
	{
		USER_SAVE = 0,
		USER_LOAD,
		USER_MATERIAL_CHANGE,
		USER_ENTITY_CHANGE,
		NUM_CUSTOM_EVENTS // Siempre al final, para contar el número de eventos
	};



#define EVENT_CLASS_TYPE(type) static smol::EventType GetStaticType() { return  smol::EventType::##type; } \
								virtual  smol::EventType GetEventType() const override { return GetStaticType(); } \
								virtual const char* GetName() const override { return #type; }

	class CustomEvent
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

	public:
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool, T&>;

	public:
		EventDispatcher(SDL_Event& event) : m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.type == T::type)
			{
				m_Event.code = func(*(T*)&m_Event);
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