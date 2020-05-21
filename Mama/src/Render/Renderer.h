#pragma once
#include "Display/Shader.h"
#include "Display/Camera.h"
#include "Effects/ShadowMap.h"

struct Lights
{
	glm::vec3 position[7] = { glm::vec3(-7.02779, 14.351f, -14.282f),
								glm::vec3(2.0601f, 14.351f, -13.435f),
								glm::vec3(7.9922f, 11.784f, -4.0616f),
								glm::vec3(24.048f, 11.784f, -4.0616f),
								glm::vec3(13.713f, 16.197f, 8.708f),
								glm::vec3(9.1793f, 17.973f, 20.621f),
								glm::vec3(-0.040154f, 23.126f, 36.366f)
	};
};

class Renderer
{
public:
	void Create(uint32_t width, uint32_t height, Camera* camera);
	void SetProps();
	void renderDefault(Shader& shader);
	void renderSimpleShadow(Shader& shader, glm::vec3& lightPos, bool shadow, float far_plane, ShadowMap* map);
	void renderSimpleLight(Shader& shader, glm::vec3& lightPos, bool shadow, float far_plane, ShadowMap* map);
	void renderLight(Shader& shader);
	void renderTransparent(Shader &shader, float &brightness);

private:
	uint32_t m_Width, m_Height;
	Camera* m_Camera;

	Lights m_Lights;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_View;

};