#pragma once

#include "Event.h"
#include "SDL_keycode.h"


namespace smol {
	class KeyEvent : public SDL_Event
	{
	public: 
		inline SDL_Keycode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(SDL_Keycode keycode) : m_KeyCode(keycode) {}

		SDL_Keycode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public: 
		KeyPressedEvent(const SDL_Keycode keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "(repeat :" << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(SDL_Keycode keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};


	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(SDL_Keycode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}