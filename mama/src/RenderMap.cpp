#include "Camera.h"
#include "Shader.h"
#include "Model.h"

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

//---------------------------------------------------------variable declaration--------------------------------------------------------------------------
//Shader display properties
glm::mat4 projectionMatrix;
glm::mat4 view;
//-------------------------------------------------------------------------------------------------------------------------------------------------------

/** set up the properties for the light shader */
void LightShaderSetup(Shader& shader, Camera& camera, GLint& width, GLint& height, float& brightness)
{
	glViewport(0.0f, 0.0f, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear everything that was before

	shader.use();
	projectionMatrix = glm::perspective(glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 100.0f);
	view = camera.getViewMatrix();
	//camera.position = camera.player->position + glm::vec3(0, 0, 10);

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera.position);
	shader.setVec3("ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("brightness", brightness, brightness, brightness);
	glActiveTexture(GL_TEXTURE1);
}

/** set up the properties for the transparent shader*/
void TransparentShaderSetup(Shader& shader, Camera& camera, GLint& width, GLint& height, float& brightness)
{
	shader.use();

	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera.position);
	shader.setVec3("ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("brightness", brightness, brightness, brightness);
	glActiveTexture(GL_TEXTURE1);
}