#pragma once

#include "MamaCore/Core.h"

#include <string>
#include <functional>

namespace Mama {



	enum class EventType
	{
		None,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// filter events
	// BIT_FIELD is definded in core.h
	enum EventCategory
	{
		None						= 0,
		EventCategoryApplication	= BIT_FIELD(0),
		EventCategoryInput			= BIT_FIELD(1),
		EventCategoryKeyboard		= BIT_FIELD(2),
		EventCategoryMouse			= BIT_FIELD(3),
		EventCategoryMouseButton	= BIT_FIELD(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type;}\
																virtual EventType GetEventType() const override { return GetStaticType(); }\
																virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class MAMA_API Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0; //ToDo only use for Debug
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool isInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			:m_Event(event)
		{

		}
		//ToDo: make sure to add typesafety that what comes in is an actual event!
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator << (std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

