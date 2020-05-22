#include "stdafx.h"
#include "Renderer.h"
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


//-------------------------------------------------------------------------------------------------------------------------------------------------------

void Renderer::Create(uint32_t width, uint32_t height, Camera* camera)
{
	m_Width = width;
	m_Height = height;
	m_Camera = camera;
}

void Renderer::renderSimpleShadow(Shader& shader, glm::vec3& lightPos,bool shadow, float far_plane, ShadowMap* map)
{
	shader.use();

	shader.setMat4("projection", m_ProjectionMatrix);
	shader.setMat4("view", m_View);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", m_Camera->m_Position);
	shader.setInt("shadows", shadow);
	shader.setFloat("far_plane", far_plane);
	shader.setFloat("shininess", 32.0f);
	
	shader.setVec3("pointLights[0].position", m_Lights.position[0]);
	shader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[0].diffuse", 0.6f, 0.6f, 0.6f);
	shader.setVec3("pointLights[0].specular", 0, 0, 0);
	shader.setFloat("pointLights[0].constant", 0.0f);
	shader.setFloat("pointLights[0].linear", 0.09);
	shader.setFloat("pointLights[0].quadratic", 0.032);

	shader.setVec3("pointLights[1].position", m_Lights.position[1]);
	shader.setVec3("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[1].diffuse", 0.6f, 0.6f, 0.6f);
	shader.setVec3("pointLights[1].specular", 0, 0, 0);
	shader.setFloat("pointLights[1].constant", 0.1f);
	shader.setFloat("pointLights[1].linear", 0.09);
	shader.setFloat("pointLights[1].quadratic", 0.032);

	shader.setVec3("pointLights[2].position", m_Lights.position[2]);
	shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[2].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[1].specular", 0.5, 0.5, 0.5);
	shader.setFloat("pointLights[2].constant", 0.0f);
	shader.setFloat("pointLights[2].linear", 0.09);
	shader.setFloat("pointLights[2].quadratic", 0.032);

	shader.setVec3("pointLights[3].position", m_Lights.position[3]);
	shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[3].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[3].specular", 1, 1, 1);
	shader.setFloat("pointLights[3].constant", 0.1f);
	shader.setFloat("pointLights[3].linear", 0.09);
	shader.setFloat("pointLights[3].quadratic", 0.032);

	shader.setVec3("pointLights[4].position", m_Lights.position[4]);
	shader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[4].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[4].specular", 0, 0, 0);
	shader.setFloat("pointLights[4].constant", 0.0f);
	shader.setFloat("pointLights[4].linear", 0.09);
	shader.setFloat("pointLights[4].quadratic", 0.032);

	shader.setVec3("pointLights[5].position", m_Lights.position[5]);
	shader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[5].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[5].specular", 0, 0, 0);
	shader.setFloat("pointLights[5].constant", 0.0f);
	shader.setFloat("pointLights[5].linear", 0.09);
	shader.setFloat("pointLights[5].quadratic", 0.032);

	shader.setVec3("pointLights[6].position", m_Lights.position[6]);
	shader.setVec3("pointLights[6].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[6].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[6].specular", 0, 0, 0);
	shader.setFloat("pointLights[6].constant", 0.0f);
	shader.setFloat("pointLights[6].linear", 0.09);
	shader.setFloat("pointLights[6].quadratic", 0.032);

	shader.setVec3("pointLights[7].position", m_Lights.position[7]);
	shader.setVec3("pointLights[7].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[7].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[7].specular", 0, 0, 0);
	shader.setFloat("pointLights[7].constant", 0.0f);
	shader.setFloat("pointLights[7].linear", 0.09);
	shader.setFloat("pointLights[7].quadratic", 0.032);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, map->GetMap());
}

void Renderer::renderSimpleLight(Shader& shader, glm::vec3& lightPos, bool shadow, float far_plane, ShadowMap* map)
{
	//glViewport(0.0f, 0.0f, width, height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	shader.use();

	shader.setMat4("projection", m_ProjectionMatrix);
	shader.setMat4("view", m_View);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", m_Camera->m_Position);
	shader.setInt("shadows", shadow);
	shader.setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, map->GetMap());
}

void Renderer::SetProps()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_Camera->m_Zoom), (float)m_Width / (float)m_Height, 0.1f, 100.0f);
	m_View = m_Camera->getViewMatrix();
}

void Renderer::renderDefault(Shader& shader)
{
	shader.use();

	shader.setMat4("projection", m_ProjectionMatrix);
	shader.setMat4("view", m_View);
	shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glActiveTexture(GL_TEXTURE0);
}

void Renderer::renderLight(Shader& shader)
{
	Lights lights;

	shader.use();
	shader.setVec3("viewPos", m_Camera->m_Position);
	shader.setFloat("shininess", 32.0f);

	//shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	shader.setVec3("pointLights[0].position", m_Lights.position[0]);
	shader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[0].diffuse", 0.6f, 0.6f, 0.6f);
	shader.setVec3("pointLights[0].specular", 0, 0, 0);
	shader.setFloat("pointLights[0].constant", 0.0f);
	shader.setFloat("pointLights[0].linear", 0.09);
	shader.setFloat("pointLights[0].quadratic", 0.032);

	shader.setVec3("pointLights[1].position", m_Lights.position[1]);
	shader.setVec3("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("pointLights[1].diffuse", 0.6f, 0.6f, 0.6f);
	shader.setVec3("pointLights[1].specular", 0, 0, 0);
	shader.setFloat("pointLights[1].constant", 0.1f);
	shader.setFloat("pointLights[1].linear", 0.09);
	shader.setFloat("pointLights[1].quadratic", 0.032);

	shader.setVec3("pointLights[2].position", m_Lights.position[2]);
	shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[2].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[1].specular", 0.5, 0.5, 0.5);
	shader.setFloat("pointLights[2].constant", 0.0f);
	shader.setFloat("pointLights[2].linear", 0.09);
	shader.setFloat("pointLights[2].quadratic", 0.032);

	shader.setVec3("pointLights[3].position", m_Lights.position[3]);
	shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[3].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[3].specular", 1, 1, 1);
	shader.setFloat("pointLights[3].constant", 0.1f);
	shader.setFloat("pointLights[3].linear", 0.09);
	shader.setFloat("pointLights[3].quadratic", 0.032);

	shader.setVec3("pointLights[4].position", m_Lights.position[4]);
	shader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[4].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[4].specular", 0, 0, 0);
	shader.setFloat("pointLights[4].constant", 0.0f);
	shader.setFloat("pointLights[4].linear", 0.09);
	shader.setFloat("pointLights[4].quadratic", 0.032);

	shader.setVec3("pointLights[5].position", m_Lights.position[5]);
	shader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[5].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[5].specular", 0, 0, 0);
	shader.setFloat("pointLights[5].constant", 0.0f);
	shader.setFloat("pointLights[5].linear", 0.09);
	shader.setFloat("pointLights[5].quadratic", 0.032);

	shader.setVec3("pointLights[6].position", m_Lights.position[6]);
	shader.setVec3("pointLights[6].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[6].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[6].specular", 0, 0, 0);
	shader.setFloat("pointLights[6].constant", 0.0f);
	shader.setFloat("pointLights[6].linear", 0.09);
	shader.setFloat("pointLights[6].quadratic", 0.032);

	shader.setVec3("pointLights[7].position", m_Lights.position[7]);
	shader.setVec3("pointLights[7].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[7].diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("pointLights[7].specular", 0, 0, 0);
	shader.setFloat("pointLights[7].constant", 0.0f);
	shader.setFloat("pointLights[7].linear", 0.09);
	shader.setFloat("pointLights[7].quadratic", 0.032);

	shader.setMat4("projection", m_ProjectionMatrix);
	shader.setMat4("view", m_View);

	glActiveTexture(GL_TEXTURE0);
}


///** set up the properties for the transparent shader*/
void Renderer::renderTransparent(Shader& shader, float& brightness)
{
	shader.use();

	shader.setMat4("projection", m_ProjectionMatrix);
	shader.setMat4("view", m_View);
	shader.setVec3("viewPos", m_Camera->m_Position);
	shader.setVec3("ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("brightness", brightness, brightness, brightness);
	glActiveTexture(GL_TEXTURE0);
}

