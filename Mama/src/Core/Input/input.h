#pragma once
#include "Core/Core.h"

namespace Mama
{
	class Input
	{
	public:
		inline static bool isKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
	
		inline static bool isMousePressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;

		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetXMouseImpl() = 0;
		virtual float GetYMouseImpl() = 0;

	private:
		static Input* s_Instance;
	};
}