#pragma once
#include "Display/Shader.h"
#include "Display/Camera.h"
#include "Effects/ShadowMap.h"
#include "Effects/FrustumCulling.h"


struct Lights
{
	glm::vec3 position[8] = {	glm::vec3(-7.02779, 14.351f, -14.282f),
								glm::vec3(2.0601f, 14.351f, -13.435f),
								glm::vec3(7.9922f, 11.784f, -4.0616f),
								glm::vec3(24.048f, 11.784f, -4.0616f),
								glm::vec3(13.713f, 16.197f, 8.708f),
								glm::vec3(9.1793f, 17.973f, 20.621f),
								glm::vec3(-0.040154f, 23.126f, 36.366f),
								glm::vec3(1.6152f,14.315f, -22.578f)
	};
};

class Renderer
{
public:
	void Create(uint32_t width, uint32_t height, Camera* camera);
	void SetProps(glm::vec3 brightness);
	void renderDefault(Shader& shader);
	void renderSimpleShadow(Shader& shader, glm::vec3& lightPos, bool shadow, float far_plane, ShadowMap* map);
	void renderSimpleLight(Shader& shader, glm::vec3& lightPos, bool shadow, float far_plane, ShadowMap* map);
	void renderLight(Shader& shader);
	void renderTransparent(Shader &shader, float &brightness);
	void renderBones(Shader& shader);
	bool isFrustum(Model &model, glm::mat4 matrix, bool off);

private:
	uint32_t m_Width, m_Height;
	Camera* m_Camera;
	FrustumCulling* m_Frustum;

	Lights m_Lights;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_View;

	glm::vec3 m_Brightness;

};