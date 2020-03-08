#pragma once

#include "Core/Core.h"
#include "Events/Event.h"

namespace Mama {
	class MAMA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer"); //debug only
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}

