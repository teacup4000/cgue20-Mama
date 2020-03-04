#pragma once

#include "Event.h"

namespace Mama
{
	class MAMA_API WindowResizeEvent : public Event
	{
		public:
			WindowResizeEvent(unsigned int width, unsigned int height)
				:m_Width(width), m_Height(height) {}
		
			inline unsigned int GetWidth() const { return m_Width; }	
			inline unsigned int GetHeight() const { return m_Height; }

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
				return ss.str();
			}

			EVENT_TYPE(WindowResize)
			EVENT_CATEGORY(EventCategoryApplication)

		private:
			unsigned int m_Width, m_Height;
	};

	class MAMA_API WindowCloseEvent : public Event
	{
		public:
			WindowCloseEvent() {}

			EVENT_TYPE(WindowClose)
			EVENT_CATEGORY(EventCategoryApplication)
	};


}