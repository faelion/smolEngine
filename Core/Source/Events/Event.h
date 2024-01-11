#pragma once
#include "smolPCH.h"
#include "SDL_events.h"


#define PUSH_CUSTOM_EVENT(eventType) \
    SDL_UserEvent customEvent; \
    SDL_zero(customEvent); \
    customEvent.data1 = eventType; \
    customEvent.type = eventType->GetEventType(); \
    customEvent.user.code = 0; \
    SDL_PushEvent(&customEvent);

#define RELEASE_EVENT(e) \
	if(e.user.data1 != nullptr) \
	delete e.user.data1; \
	if(e.user.data2 != nullptr) \
	delete e.user.data2; \


namespace smol {

	enum class EventType // Custom Event Type
	{
		USER_SAVE = SDL_USEREVENT,
		USER_LOAD,
		USER_MATERIAL_CHANGE,
		USER_ENTITY_CHANGE,
		NUM_CUSTOM_EVENTS // Siempre al final, para contar el número de eventos
	};

	inline bool isCustomEvent(const SDL_Event& e)
	{
		if (e.type >= SDL_USEREVENT && e.type < (Uint32)EventType::NUM_CUSTOM_EVENTS)
			return true;
		else
			return false;
	}



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
	public:
		EventDispatcher(SDL_Event& event) : m_Event(event)
		{
		}

		// F will be deduced by the compiler
		template<unsigned int et, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.type == et)
			{
				m_Event.user.code = func(m_Event);
				return true;
			}
			return false;
		}
	private:
		SDL_Event& m_Event;
	};
}