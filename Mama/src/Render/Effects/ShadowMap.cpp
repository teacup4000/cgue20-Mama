#include "stdafx.h"
#include "ShadowMap.h"


void ShadowMap::GenerateDepthMap(float nearPlane, float farPlane)
{
	m_Nearplane = nearPlane;
	m_Farplane = farPlane;

	glGenFramebuffers(1, &m_DepthMapFBO);
	glGenTextures(1, &m_DepthMapCube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthMapCube);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach the Texture to the depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthMapCube, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ShadowMap::GenerateCubeMap(glm::vec3 &lightPos)
{
	m_ShadowProjection = glm::perspective(glm::radians(90.0f), (float)m_Width / (float)m_Height, m_Nearplane, m_Farplane);

	m_ShadowTransform.push_back(m_ShadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_ShadowTransform.push_back(m_ShadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_ShadowTransform.push_back(m_ShadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_ShadowTransform.push_back(m_ShadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	m_ShadowTransform.push_back(m_ShadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_ShadowTransform.push_back(m_ShadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}
void ShadowMap::RenderDepthMap(Shader &shader, glm::vec3 &lightPos)
{
	glViewport(0.0f, 0.0f, m_Width, m_Height);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	shader.use();
	for (unsigned int i = 0; i < 6; ++i)
	{
		shader.setMat4("shadowMatrices[" + std::to_string(i) + "]", m_ShadowTransform[i]);
	}
	shader.setFloat("far_plane", m_Farplane);
	shader.setVec3("lightPos", lightPos);
}