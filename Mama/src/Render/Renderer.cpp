#include "Display/Camera.h"
#include "Display/Shader.h"
#include "Display/Model.h"

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

//---------------------------------------------------------variable declaration--------------------------------------------------------------------------
const unsigned int shadow_width = 1024, shadow_height = 1024;
unsigned int depthMapFBO;
unsigned int depthMapCube;

float near_plane = 1.0f;
float far_plane = 25.0f;
glm::mat4 shadowProjection;
glm::mat4 projectionMatrix;
glm::mat4 view;
std::vector<glm::mat4> shadowTransform;

struct Lights 
{
	glm::vec3 position[7] =	{	glm::vec3(-6.9473f, 14.351f, -14.282f),
								glm::vec3(2.0601f, 14.351f, -13.435f),
								glm::vec3(7.9922f, 11.784f, -4.0616f),
								glm::vec3(25.048f, 11.784f, -4.0616f),
								glm::vec3(13.713f, 16.197f, 8.708f),
								glm::vec3(9.1793f, 17.973f, 20.621f),
								glm::vec3(-0.040154f, 23.126f, 36.366f)
							};
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------

void generateDepthMap()
{
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMapCube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapCube);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach the Texture to the depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapCube, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void createCubeMapMatrix(glm::vec3& lightPos)
{
	shadowProjection = glm::perspective(glm::radians(90.0f), (float)shadow_width / (float)shadow_height, near_plane, far_plane);

	shadowTransform.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransform.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransform.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransform.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransform.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransform.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}

void renderDepthMap(Shader& shader, glm::vec3& lightPos)
{
	glViewport(0.0f, 0.0f, shadow_width, shadow_height);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	shader.use();
	for (unsigned int i = 0; i < 6; ++i)
	{
		shader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransform[i]);
	}
	shader.setFloat("far_plane", far_plane);
	shader.setVec3("lightPos", lightPos);
}

void renderDefault(Shader& shader, Camera* camera, glm::vec3& lightPos, GLint& width, GLint& height)
{
	glViewport(0.0f, 0.0f, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	projectionMatrix = glm::perspective(glm::radians(camera->m_Zoom), (float)width / (float)height, 0.1f, 100.0f);
	view = camera->getViewMatrix();

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", camera->m_Position);
	shader.setInt("shadows", true);
	shader.setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapCube);
}

void renderLight(Shader& shader, Camera* camera, GLint &width, GLint &height)
{
	Lights lights;

	shader.use();
	shader.setVec3("viewPos", camera->m_Position);
	shader.setFloat("shininess", 32.0f);

	//shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	shader.setVec3("pointLights[0].position", lights.position[0]);
	shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[0].constant", 1.0f);
	shader.setFloat("pointLights[0].linear", 0.09);
	shader.setFloat("pointLights[0].quadratic", 0.032);

	shader.setVec3("pointLights[1].position", lights.position[1]);
	shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[1].constant", 1.0f);
	shader.setFloat("pointLights[1].linear", 0.09);
	shader.setFloat("pointLights[1].quadratic", 0.032);

	shader.setVec3("pointLights[2].position", lights.position[2]);
	shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[2].constant", 1.0f);
	shader.setFloat("pointLights[2].linear", 0.09);
	shader.setFloat("pointLights[2].quadratic", 0.032);

	shader.setVec3("pointLights[3].position", lights.position[3]);
	shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[3].constant", 1.0f);
	shader.setFloat("pointLights[3].linear", 0.09);
	shader.setFloat("pointLights[3].quadratic", 0.032);

	shader.setVec3("pointLights[4].position", lights.position[4]);
	shader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[4].constant", 1.0f);
	shader.setFloat("pointLights[4].linear", 0.09);
	shader.setFloat("pointLights[4].quadratic", 0.032);

	shader.setVec3("pointLights[5].position", lights.position[5]);
	shader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[5].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[5].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[5].constant", 1.0f);
	shader.setFloat("pointLights[5].linear", 0.09);
	shader.setFloat("pointLights[5].quadratic", 0.032);

	shader.setVec3("pointLights[6].position", lights.position[6]);
	shader.setVec3("pointLights[6].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[6].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[6].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[6].constant", 1.0f);
	shader.setFloat("pointLights[6].linear", 0.09);
	shader.setFloat("pointLights[6].quadratic", 0.032);

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);
	//shader.setVec3("lightPos", lightPos);
	//shader.setFloat("far_plane", far_plane);

	glActiveTexture(GL_TEXTURE0);
}

///** set up the properties for the transparent shader*/
//void TransparentShaderSetup(Shader& shader, Camera& camera, GLint& width, GLint& height, float& brightness)
//{
//	shader.use();
//
//	shader.setMat4("projection", projectionMatrix);
//	shader.setMat4("view", view);
//	shader.setVec3("viewPos", camera.position);
//	shader.setVec3("ambient", 0.0f, 0.0f, 0.0f);
//	shader.setVec3("brightness", brightness, brightness, brightness);
//	glActiveTexture(GL_TEXTURE1);
//}

