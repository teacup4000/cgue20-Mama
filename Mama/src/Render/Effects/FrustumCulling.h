#pragma once
#include <glm.hpp>
#include <numeric> 

#include "Render/Display/Model.h"
#include "Render/Display/Camera.h"

class FrustumCulling
{
public:

	enum Side
	{
		RIGHT	= 0,
		LEFT	= 1,
		BOTTOM	= 2,
		TOP		= 3,
		BACK	= 4,
		FRONT	= 5
	};

	enum Plane
	{
		A		= 0,
		B		= 1,
		C		= 2,
		D		= 3
	};

	FrustumCulling()
	{
		m_Off = false;
	}

	bool isFrustum(Model &model, glm::mat4 matrix, glm::mat4 proj, bool off);

	void CalculateFrustum(glm::mat4 matrix, glm::mat4 proj);
	
	void Normalize(int side);

	bool FrustumBox(glm::vec3 center, float radius);
	bool FrustumRect(glm::vec3 center, float x, float y, float z);

private:
	float m_Frustum[6][4];
	bool m_Off;
};