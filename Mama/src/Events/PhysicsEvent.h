#pragma once

#include "Event.h"

namespace Mama
{
	class CollisionEvent : public Event 
	{
		//has to be implemented depending on physics engine we use.
		/*
			ToDo: something like
			CollisionEvent(Entity a, Entity b) : m_A(a), m_B(b) {}
		*/
		
		EVENT_TYPE(Collision)
		EVENT_CATEGORY(EventCategoryPhysics)
	};

}