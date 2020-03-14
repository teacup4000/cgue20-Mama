#pragma once

#include "Event.h"

namespace Mama
{
	class KeyEvent : public Event
	{
		public:
			inline int GetKeyCode() const { return m_KeyCode; }

			EVENT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
		
		protected:
			KeyEvent(int keycode)
				: m_KeyCode(keycode) {}

			int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
		public:
			KeyPressedEvent(int keycode, int repeatCount)
				: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

			inline int GetRepeatCount() const { return m_RepeatCount;  }
		
			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
				return ss.str();
			}

			EVENT_TYPE(KeyPressed)

		private:
			int m_RepeatCount; //Maybe no need for counting how long a key has been pressed
	
	};

	class KeyReleasedEvent : public KeyEvent //Maybe just KeyPressedEvent needed
	{
		public:
			KeyReleasedEvent(int keycode)
				: KeyEvent(keycode) {}

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "KeyReleasedEvent: " << m_KeyCode;
				return ss.str();
			}

			EVENT_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_TYPE(KeyTyped)
	};
}