#include "stdafx.h"
#include "FrustumCulling.h"

bool FrustumCulling::isFrustum(Model &model, glm::mat4 matrix, glm::mat4 proj, bool off)
{
	if (off)
		return true;

	CalculateFrustum(matrix, proj);
	float size = model.GetDistance() / 4;
	return FrustumBox(model.GetPosition(), size);
}

void FrustumCulling::CalculateFrustum(glm::mat4 matrix, glm::mat4 proj)
{
	float clip[16];

	clip[0] = matrix[0].x * proj[0].x + matrix[0].y * proj[1].x + matrix[0].z * proj[2].x + matrix[0].w * proj[3].x;
	clip[1] = matrix[0].x * proj[0].y + matrix[0].y * proj[1].y + matrix[0].z * proj[2].y + matrix[0].w * proj[3].y;
	clip[2] = matrix[0].x * proj[0].z + matrix[0].y * proj[1].z + matrix[0].z * proj[2].z + matrix[0].w * proj[3].z;
	clip[3] = matrix[0].x * proj[0].w + matrix[0].y * proj[1].w + matrix[0].z * proj[2].w + matrix[0].w * proj[3].w;

	clip[4] = matrix[1].x * proj[0].x + matrix[1].y *proj[1].x + matrix[1].z * proj[2].x + matrix[1].w * proj[3].x;
	clip[5] = matrix[1].x * proj[0].y + matrix[1].y *proj[1].y + matrix[1].z * proj[2].y + matrix[1].w * proj[3].y;
	clip[6] = matrix[1].x * proj[0].z + matrix[1].y *proj[1].z + matrix[1].z * proj[2].z + matrix[1].w * proj[3].z;
	clip[7] = matrix[1].x * proj[0].w + matrix[1].y *proj[1].w + matrix[1].z * proj[2].w + matrix[1].w * proj[3].w;

	clip[8] = matrix[2].x * proj[0].x + matrix[2].y *proj[1].x + matrix[2].z * proj[2].x + matrix[2].w * proj[3].x;
	clip[9] = matrix[2].x * proj[0].y + matrix[2].y *proj[1].y + matrix[2].z * proj[2].y + matrix[2].w * proj[3].y;
	clip[10] = matrix[2].x * proj[0].z + matrix[2].y *proj[1].z + matrix[2].z * proj[2].z + matrix[2].w * proj[3].z;
	clip[11] = matrix[2].x * proj[0].w + matrix[2].y *proj[1].w + matrix[2].z * proj[2].w + matrix[2].w * proj[3].w;

	clip[12] = matrix[3].x * proj[0].x + matrix[3].y *proj[1].x + matrix[3].z * proj[2].x + matrix[3].w * proj[3].x;
	clip[13] = matrix[3].x * proj[0].y + matrix[3].y *proj[1].y + matrix[3].z * proj[2].y + matrix[3].w * proj[3].y;
	clip[14] = matrix[3].x * proj[0].z + matrix[3].y *proj[1].z + matrix[3].z * proj[2].z + matrix[3].w * proj[3].z;
	clip[15] = matrix[3].x * proj[0].w + matrix[3].y *proj[1].w + matrix[3].z * proj[2].w + matrix[3].w * proj[3].w;

	m_Frustum[RIGHT][A] = clip[3] - clip[0];
	m_Frustum[RIGHT][B] = clip[7] - clip[4];
	m_Frustum[RIGHT][C] = clip[11] - clip[8];
	m_Frustum[RIGHT][D] = clip[15] - clip[12];

	Normalize(RIGHT);

	m_Frustum[LEFT][A] = clip[3] + clip[0];
	m_Frustum[LEFT][B] = clip[7] + clip[4];
	m_Frustum[LEFT][C] = clip[11] + clip[8];
	m_Frustum[LEFT][D] = clip[15] + clip[12];

	Normalize(LEFT);

	m_Frustum[BOTTOM][A] = clip[3] + clip[1];
	m_Frustum[BOTTOM][B] = clip[7] + clip[5];
	m_Frustum[BOTTOM][C] = clip[11] + clip[9];
	m_Frustum[BOTTOM][D] = clip[15] + clip[13];

	Normalize(BOTTOM);

	m_Frustum[TOP][A] = clip[3] - clip[1];
	m_Frustum[TOP][B] = clip[7] - clip[5];
	m_Frustum[TOP][C] = clip[11] - clip[9];
	m_Frustum[TOP][D] = clip[15] - clip[13];

	Normalize(TOP);

	m_Frustum[BACK][A] = clip[3] - clip[2];
	m_Frustum[BACK][B] = clip[7] - clip[6];
	m_Frustum[BACK][C] = clip[11] - clip[10];
	m_Frustum[BACK][D] = clip[15] - clip[14];

	Normalize(BACK);

	m_Frustum[FRONT][A] = clip[3] + clip[2];
	m_Frustum[FRONT][B] = clip[7] + clip[6];
	m_Frustum[FRONT][C] = clip[11] + clip[10];
	m_Frustum[FRONT][D] = clip[15] + clip[14];

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
		if (m_Frustum[i][A] * (center.x - radius) + m_Frustum[i][B] * (center.y - radius) + m_Frustum[i][C] * (center.z - radius) + m_Frustum[i][D] > 0)
			continue;
		if (m_Frustum[i][A] * (center.x + radius) + m_Frustum[i][B] * (center.y - radius) + m_Frustum[i][C] * (center.z - radius) + m_Frustum[i][D] > 0)
			continue;
		if (m_Frustum[i][A] * (center.x - radius) + m_Frustum[i][B] * (center.y + radius) + m_Frustum[i][C] * (center.z - radius) + m_Frustum[i][D] > 0)
			continue;
		if (m_Frustum[i][A] * (center.x + radius) + m_Frustum[i][B] * (center.y + radius) + m_Frustum[i][C] * (center.z - radius) + m_Frustum[i][D] > 0)
			continue;
		if (m_Frustum[i][A] * (center.x - radius) + m_Frustum[i][B] * (center.y - radius) + m_Frustum[i][C] * (center.z + radius) + m_Frustum[i][D] > 0)
			continue;
		if (m_Frustum[i][A] * (center.x + radius) + m_Frustum[i][B] * (center.y - radius) + m_Frustum[i][C] * (center.z + radius) + m_Frustum[i][D] > 0)
			continue;
		if (m_Frustum[i][A] * (center.x - radius) + m_Frustum[i][B] * (center.y + radius) + m_Frustum[i][C] * (center.z + radius) + m_Frustum[i][D] > 0)
			continue;
		if (m_Frustum[i][A] * (center.x + radius) + m_Frustum[i][B] * (center.y + radius) + m_Frustum[i][C] * (center.z + radius) + m_Frustum[i][D] > 0)
			continue;

		return false;
	}

	return true;
}
