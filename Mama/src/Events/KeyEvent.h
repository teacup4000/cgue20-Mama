#pragma once

#include "Event.h"

namespace Mama
{
	class MAMA_API KeyEvent : public Event
	{
		public:
			inline int GetKeyCode() const { return m_KeyCode; }

			EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
		
		protected:
			KeyEvent(int keycode)
				: m_KeyCode(keycode) {}

			int m_KeyCode;
	};

	class MAMA_API KeyPressedEvent : public KeyEvent
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

			EVENT_CLASS_TYPE(KeyPressed)

		private:
			int m_RepeatCount; //Maybe no need for counting how long a key has been pressed
	
	};

	class MAMA_API KeyReleasedEvent : public KeyEvent //Maybe just KeyPressedEvent needed
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

			EVENT_CLASS_TYPE(KeyReleased)
	};
}