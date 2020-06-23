#include "stdafx.h"
#include "FrustumCulling.h"

bool FrustumCulling::isFrustum(Model &model, glm::mat4 matrix, glm::mat4 proj, bool off)
{
	if (off)
		return true;

	CalculateFrustum(matrix, proj);
	float size = model.GetDistance();
	return FrustumBox(model.GetMinPos(), size) || FrustumBox(model.GetMaxPos(), size);
}

void FrustumCulling::CalculateFrustum(glm::mat4 modelMat, glm::mat4 proj)
{
	float matrix[16];

	matrix[0] = modelMat[0].x * proj[0].x + modelMat[0].y * proj[1].x + modelMat[0].z * proj[2].x + modelMat[0].w * proj[3].x;
	matrix[1] = modelMat[0].x * proj[0].y + modelMat[0].y * proj[1].y + modelMat[0].z * proj[2].y + modelMat[0].w * proj[3].y;
	matrix[2] = modelMat[0].x * proj[0].z + modelMat[0].y * proj[1].z + modelMat[0].z * proj[2].z + modelMat[0].w * proj[3].z;
	matrix[3] = modelMat[0].x * proj[0].w + modelMat[0].y * proj[1].w + modelMat[0].z * proj[2].w + modelMat[0].w * proj[3].w;

	matrix[4] = modelMat[1].x * proj[0].x + modelMat[1].y *proj[1].x + modelMat[1].z * proj[2].x + modelMat[1].w * proj[3].x;
	matrix[5] = modelMat[1].x * proj[0].y + modelMat[1].y *proj[1].y + modelMat[1].z * proj[2].y + modelMat[1].w * proj[3].y;
	matrix[6] = modelMat[1].x * proj[0].z + modelMat[1].y *proj[1].z + modelMat[1].z * proj[2].z + modelMat[1].w * proj[3].z;
	matrix[7] = modelMat[1].x * proj[0].w + modelMat[1].y *proj[1].w + modelMat[1].z * proj[2].w + modelMat[1].w * proj[3].w;

	matrix[8] = modelMat[2].x * proj[0].x + modelMat[2].y *proj[1].x + modelMat[2].z * proj[2].x + modelMat[2].w * proj[3].x;
	matrix[9] = modelMat[2].x * proj[0].y + modelMat[2].y *proj[1].y + modelMat[2].z * proj[2].y + modelMat[2].w * proj[3].y;
	matrix[10] = modelMat[2].x * proj[0].z + modelMat[2].y *proj[1].z + modelMat[2].z * proj[2].z + modelMat[2].w * proj[3].z;
	matrix[11] = modelMat[2].x * proj[0].w + modelMat[2].y *proj[1].w + modelMat[2].z * proj[2].w + modelMat[2].w * proj[3].w;

	matrix[12] = modelMat[3].x * proj[0].x + modelMat[3].y *proj[1].x + modelMat[3].z * proj[2].x + modelMat[3].w * proj[3].x;
	matrix[13] = modelMat[3].x * proj[0].y + modelMat[3].y *proj[1].y + modelMat[3].z * proj[2].y + modelMat[3].w * proj[3].y;
	matrix[14] = modelMat[3].x * proj[0].z + modelMat[3].y *proj[1].z + modelMat[3].z * proj[2].z + modelMat[3].w * proj[3].z;
	matrix[15] = modelMat[3].x * proj[0].w + modelMat[3].y *proj[1].w + modelMat[3].z * proj[2].w + modelMat[3].w * proj[3].w;

	//get planes from projection Matrix
	m_Frustum[RIGHT][A] = matrix[3] - matrix[0];
	m_Frustum[RIGHT][B] = matrix[7] - matrix[4];
	m_Frustum[RIGHT][C] = matrix[11] - matrix[8];
	m_Frustum[RIGHT][D] = matrix[15] - matrix[12];

	m_Frustum[LEFT][A] = matrix[3] + matrix[0];
	m_Frustum[LEFT][B] = matrix[7] + matrix[4];
	m_Frustum[LEFT][C] = matrix[11] + matrix[8];
	m_Frustum[LEFT][D] = matrix[15] + matrix[12];

	m_Frustum[BOTTOM][A] = matrix[3] + matrix[1];
	m_Frustum[BOTTOM][B] = matrix[7] + matrix[5];
	m_Frustum[BOTTOM][C] = matrix[11] + matrix[9];
	m_Frustum[BOTTOM][D] = matrix[15] + matrix[13];

	m_Frustum[TOP][A] = matrix[3] - matrix[1];
	m_Frustum[TOP][B] = matrix[7] - matrix[5];
	m_Frustum[TOP][C] = matrix[11] - matrix[9];
	m_Frustum[TOP][D] = matrix[15] - matrix[13];

	m_Frustum[BACK][A] = matrix[3] - matrix[2];
	m_Frustum[BACK][B] = matrix[7] - matrix[6];
	m_Frustum[BACK][C] = matrix[11] - matrix[10];
	m_Frustum[BACK][D] = matrix[15] - matrix[14];

	m_Frustum[FRONT][A] = matrix[3] + matrix[2];
	m_Frustum[FRONT][B] = matrix[7] + matrix[6];
	m_Frustum[FRONT][C] = matrix[11] + matrix[10];
	m_Frustum[FRONT][D] = matrix[15] + matrix[14];

	//normalize planes
	Normalize(RIGHT);
	Normalize(LEFT);
	Normalize(BOTTOM);
	Normalize(TOP);
	Normalize(BACK);
	Normalize(FRONT);
}

void FrustumCulling::Normalize(int side)
{
	float distance = (float)sqrt(pow(m_Frustum[side][A], 2) + pow(m_Frustum[side][B], 2) + pow(m_Frustum[side][C], 2));
	for (int i = 0; i < 4; i++)
	{
		m_Frustum[side][i] /= distance;
	}
}

bool FrustumCulling::FrustumBox(glm::vec3 center, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		float box1 = m_Frustum[i][A] * (center.x - radius) + m_Frustum[i][B] * (center.y - radius) + m_Frustum[i][C] * (center.z - radius) + m_Frustum[i][D];
		float box2 = m_Frustum[i][A] * (center.x + radius) + m_Frustum[i][B] * (center.y - radius) + m_Frustum[i][C] * (center.z - radius) + m_Frustum[i][D];
		float box3 = m_Frustum[i][A] * (center.x - radius) + m_Frustum[i][B] * (center.y + radius) + m_Frustum[i][C] * (center.z - radius) + m_Frustum[i][D];
		float box4 = m_Frustum[i][A] * (center.x + radius) + m_Frustum[i][B] * (center.y + radius) + m_Frustum[i][C] * (center.z - radius) + m_Frustum[i][D];
		float box5 = m_Frustum[i][A] * (center.x - radius) + m_Frustum[i][B] * (center.y - radius) + m_Frustum[i][C] * (center.z + radius) + m_Frustum[i][D];
		float box6 = m_Frustum[i][A] * (center.x + radius) + m_Frustum[i][B] * (center.y - radius) + m_Frustum[i][C] * (center.z + radius) + m_Frustum[i][D];
		float box7 = m_Frustum[i][A] * (center.x - radius) + m_Frustum[i][B] * (center.y + radius) + m_Frustum[i][C] * (center.z + radius) + m_Frustum[i][D];
		float box8 = m_Frustum[i][A] * (center.x + radius) + m_Frustum[i][B] * (center.y + radius) + m_Frustum[i][C] * (center.z + radius) + m_Frustum[i][D];

		if (box1 < 0 && box2 < 0 && box3 < 0 && box4 < 0 && box5 < 0 && box6 < 0 && box7 < 0 && box8 < 0)
			return false;
	}

	return true;
}
