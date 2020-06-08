#pragma once
#include "Render/Display/Shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
class ShadowMap
{
public:
	void GenerateDepthMap(float nearPlane, float farPlane);
	void GenerateCubeMap(glm::vec3 &lightPos);
	void RenderDepthMap(Shader &shader, glm::vec3 &lightPos);

	uint32_t GetMap() { return m_DepthMapCube; }
private:
	const uint32_t m_Width = 1024, m_Height = 1024;

	uint32_t m_DepthMapFBO;
	uint32_t m_DepthMapCube;
	glm::mat4 m_ShadowProjection;
	glm::mat4 m_ProjectionMatrix;
	std::vector<glm::mat4> m_ShadowTransform;

	float m_Farplane, m_Nearplane;

};