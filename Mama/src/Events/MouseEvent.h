#pragma once

#include "Event.h"

namespace Mama
{
	class MAMA_API MouseMovedEvent : public Event
	{
		public:
			MouseMovedEvent(float x, float y)
				: m_MouseX(x), m_MouseY(y) {}

			inline float GetX() const { return m_MouseX; }
			inline float GetY() const { return m_MouseY; }

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
				return ss.str();
			}

			EVENT_TYPE(MouseMoved)
			EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)
		
		private:
			float m_MouseX, m_MouseY;
	};

	class MAMA_API MouseButtonEvent : public Event
	{
		public:
			inline int GetMouseButton() const { return m_Button; }

			EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

		protected:
			MouseButtonEvent(int button)
				: m_Button(button) {}

			int m_Button;
	};

	class MAMA_API MouseButtonPressedEvent : public MouseButtonEvent
	{
		public:
			MouseButtonPressedEvent(int button)
				: MouseButtonEvent(button) {}

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "MouseButtonPressedEvent: " << m_Button;
				return ss.str();
			}

			EVENT_TYPE(MouseButtonPressed)
	};

	class MAMA_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
		public:
			MouseButtonReleasedEvent(int button)
				: MouseButtonEvent(button) {}

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "MouseButtonReleasedEvent: " << m_Button;
				return ss.str();
			}

			EVENT_TYPE(MouseButtonReleased)
	};
}