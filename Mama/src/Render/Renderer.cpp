#include "stdafx.h"
#include "Display/Camera.h"
#include "Display/Shader.h"
#include "Display/Model.h"
#include "Effects/ShadowMap.h"

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//---------------------------------------------------------variable declaration--------------------------------------------------------------------------

unsigned int testFBO;
unsigned int textures[2];

glm::mat4 projectionMatrix;
glm::mat4 view;

struct Lights 
{
	glm::vec3 position[7] =	{	glm::vec3(-7.02779, 14.351f, -14.282f),
								glm::vec3(2.0601f, 14.351f, -13.435f),
								glm::vec3(7.9922f, 11.784f, -4.0616f),
								glm::vec3(24.048f, 11.784f, -4.0616f),
								glm::vec3(13.713f, 16.197f, 8.708f),
								glm::vec3(9.1793f, 17.973f, 20.621f),
								glm::vec3(-0.040154f, 23.126f, 36.366f)
							};
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------

void generateTestFBO(uint32_t width, uint32_t height)
{
	glGenFramebuffers(1, &testFBO);

	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, testFBO);	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textures[1], 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void bindTestFBO(unsigned int width, unsigned int height)
{
	glViewport(0.0f, 0.0f, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, testFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderSimpleShadow(Shader& shader, Camera* camera, glm::vec3& lightPos, GLint& width, GLint& height, bool shadow, float far_plane, ShadowMap* map)
{
	Lights lights;

	shader.use();

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", camera->m_Position);
	shader.setInt("shadows", shadow);
	shader.setFloat("far_plane", far_plane);
	shader.setFloat("shininess", 32.0f);
	
	shader.setVec3("pointLights[0].position", lights.position[0]);
	shader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[0].diffuse", 0.6f, 0.6f, 0.6f);
	shader.setVec3("pointLights[0].specular", 0, 0, 0);
	shader.setFloat("pointLights[0].constant", 0.0f);
	shader.setFloat("pointLights[0].linear", 0.09);
	shader.setFloat("pointLights[0].quadratic", 0.032);

	shader.setVec3("pointLights[1].position", lights.position[1]);
	shader.setVec3("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[1].diffuse", 0.6f, 0.6f, 0.6f);
	shader.setVec3("pointLights[1].specular", 0, 0, 0);
	shader.setFloat("pointLights[1].constant", 0.1f);
	shader.setFloat("pointLights[1].linear", 0.09);
	shader.setFloat("pointLights[1].quadratic", 0.032);

	shader.setVec3("pointLights[2].position", lights.position[2]);
	shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[2].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[1].specular", 0.5, 0.5, 0.5);
	shader.setFloat("pointLights[2].constant", 0.0f);
	shader.setFloat("pointLights[2].linear", 0.09);
	shader.setFloat("pointLights[2].quadratic", 0.032);

	shader.setVec3("pointLights[3].position", lights.position[3]);
	shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[3].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[3].specular", 1, 1, 1);
	shader.setFloat("pointLights[3].constant", 0.1f);
	shader.setFloat("pointLights[3].linear", 0.09);
	shader.setFloat("pointLights[3].quadratic", 0.032);

	shader.setVec3("pointLights[4].position", lights.position[4]);
	shader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[4].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[4].specular", 0, 0, 0);
	shader.setFloat("pointLights[4].constant", 0.0f);
	shader.setFloat("pointLights[4].linear", 0.09);
	shader.setFloat("pointLights[4].quadratic", 0.032);

	shader.setVec3("pointLights[5].position", lights.position[5]);
	shader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[5].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[5].specular", 0, 0, 0);
	shader.setFloat("pointLights[5].constant", 0.0f);
	shader.setFloat("pointLights[5].linear", 0.09);
	shader.setFloat("pointLights[5].quadratic", 0.032);

	shader.setVec3("pointLights[6].position", lights.position[6]);
	shader.setVec3("pointLights[6].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[6].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[6].specular", 0, 0, 0);
	shader.setFloat("pointLights[6].constant", 0.0f);
	shader.setFloat("pointLights[6].linear", 0.09);
	shader.setFloat("pointLights[6].quadratic", 0.032);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, map->GetMap());
}

void renderSimpleLight(Shader& shader, Camera* camera, glm::vec3& lightPos, GLint& width, GLint& height, bool shadow, float far_plane, ShadowMap* map)
{
	//glViewport(0.0f, 0.0f, width, height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	shader.use();

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", camera->m_Position);
	shader.setInt("shadows", shadow);
	shader.setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, map->GetMap());
}

void setProjectionViewMatrix(Camera* camera, GLint width, GLint height)
{
	projectionMatrix = glm::perspective(glm::radians(camera->m_Zoom), (float)width / (float)height, 0.1f, 100.0f);
	view = camera->getViewMatrix();
}

void renderDefault(Shader& shader, Camera* camera, GLint& width, GLint& height)
{
	shader.use();

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);
	shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glActiveTexture(GL_TEXTURE0);
}

void postprocessTest(Shader& shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	shader.setInt("u_texture", 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
	shader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[0].diffuse", 0.6f, 0.6f, 0.6f);
	shader.setVec3("pointLights[0].specular", 0, 0, 0);
	shader.setFloat("pointLights[0].constant", 0.0f);
	shader.setFloat("pointLights[0].linear", 0.09);
	shader.setFloat("pointLights[0].quadratic", 0.032);

	shader.setVec3("pointLights[1].position", lights.position[1]);
	shader.setVec3("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[1].diffuse", 0.6f, 0.6f, 0.6f);
	shader.setVec3("pointLights[1].specular", 0, 0, 0);
	shader.setFloat("pointLights[1].constant", 0.1f);
	shader.setFloat("pointLights[1].linear", 0.09);
	shader.setFloat("pointLights[1].quadratic", 0.032);

	shader.setVec3("pointLights[2].position", lights.position[2]);
	shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[2].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[1].specular", 0.5, 0.5, 0.5);
	shader.setFloat("pointLights[2].constant", 0.0f);
	shader.setFloat("pointLights[2].linear", 0.09);
	shader.setFloat("pointLights[2].quadratic", 0.032);

	shader.setVec3("pointLights[3].position", lights.position[3]);
	shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[3].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[3].specular", 1, 1, 1);
	shader.setFloat("pointLights[3].constant", 0.1f);
	shader.setFloat("pointLights[3].linear", 0.09);
	shader.setFloat("pointLights[3].quadratic", 0.032);

	shader.setVec3("pointLights[4].position", lights.position[4]);
	shader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[4].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[4].specular", 0, 0, 0);
	shader.setFloat("pointLights[4].constant", 0.0f);
	shader.setFloat("pointLights[4].linear", 0.09);
	shader.setFloat("pointLights[4].quadratic", 0.032);

	shader.setVec3("pointLights[5].position", lights.position[5]);
	shader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[5].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[5].specular", 0, 0, 0);
	shader.setFloat("pointLights[5].constant", 0.0f);
	shader.setFloat("pointLights[5].linear", 0.09);
	shader.setFloat("pointLights[5].quadratic", 0.032);

	shader.setVec3("pointLights[6].position", lights.position[6]);
	shader.setVec3("pointLights[6].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[6].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[6].specular", 0, 0, 0);
	shader.setFloat("pointLights[6].constant", 0.0f);
	shader.setFloat("pointLights[6].linear", 0.09);
	shader.setFloat("pointLights[6].quadratic", 0.032);

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);

	glActiveTexture(GL_TEXTURE0);
}


///** set up the properties for the transparent shader*/
void TransparentShaderSetup(Shader& shader, Camera* camera, GLint& width, GLint& height, float& brightness)
{
	shader.use();

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera->m_Position);
	shader.setVec3("ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("brightness", brightness, brightness, brightness);
	glActiveTexture(GL_TEXTURE0);
}

