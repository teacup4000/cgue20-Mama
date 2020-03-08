#pragma once
#include "Core/Input/input.h"

namespace Mama
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> GetMousePosImpl() override;
		virtual float GetXMouseImpl() override;
		virtual float GetYMouseImpl() override;
	};

}