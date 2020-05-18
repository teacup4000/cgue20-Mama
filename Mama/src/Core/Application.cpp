#include "Application.h"

//#define NOMINMAX

#include <cstdio>
#include <ctime>

#include <iostream>

//------globals------
int count = 0;
//-----------------------------

glm::vec3 lightPos = glm::vec3(7.9922f, 50.0f, -4.0616f);

//render Shaders
void renderModel(Model model, Shader shader, glm::mat4 matrix);

//Shadow functions in ShadowMap.cpp
void generateDepthMap();
void createCubeMapMatrix(glm::vec3& lightPos);

void generateBloom(unsigned int width, unsigned int height);
void renderFloatingPoitBuffer(unsigned int width, unsigned int height);
void renderBlur(Shader& shader, bool& horizontal, bool& firstIt);
void renderBloomFinal(Shader& shader, bool& horizontal);
void renderDefault(Shader& shader, Camera* camera, GLint& width, GLint& height);

void renderDepthMap(Shader& shader, glm::vec3& lightPos);
void renderSimpleLight(Shader& shader, Camera* camera, glm::vec3& lightPos, GLint& width, GLint& height, bool shadow);
void renderLight(Shader& shader, Camera* camera, GLint& width, GLint& height);

void lose();

void Application::Run()
{
	//--------------locals------------
	//Frame Rate independency
	float deltaTime = 0.0f, lastFrame = 0.0f;

	//Timer variables
	float counter = 0.0f;
	float timeDiff = 0.0f;

	/*Read the settings file*/
	CreateGLFWWindow();

	Shader basic("Shader/basic.shader");
	Shader shadow("Shader/depth.shader");
	Shader pointLights("Shader/multipleLight.shader");
	Shader normal("Shader/multipleLightsNormalMap.shader");
	Shader simpleLight("Shader/light.shader");
	Shader bloom("Shader/bloom.shader");
	Shader blur("Shader/blur.shader");
	Shader bloomFinal("Shader/bloom_final.shader");
	//Shader normal("Shader/normal.shader");
	
	Model floor01("Models/Floor/Path01.obj");
	glm::mat4 path01 = glm::mat4(1.0f);

	Model floor02("Models/Floor/Path02.obj");
	glm::mat4 path02 = glm::mat4(1.0f);

	Model floor03("Models/Floor/Path03.obj");
	glm::mat4 path03 = glm::mat4(1.0f);
	
	Model wall01("Models/Walls/Wall01.obj");
	glm::mat4 wallMat01 = glm::mat4(1.0f);
	
	Model wall02("Models/Walls/Wall02.obj");
	glm::mat4 wallMat02 = glm::mat4(1.0f);
	
	Model wall03("Models/Walls/Wall03.obj");
	glm::mat4 wallMat03 = glm::mat4(1.0f);
	
	Model wall04("Models/Walls/Wall04.obj");
	glm::mat4 wallMat04 = glm::mat4(1.0f);
	
	Model wall05("Models/Walls/Wall05.obj");
	glm::mat4 wallMat05 = glm::mat4(1.0f);
	
	Model multipleLights("Models/Lights/MultipleLights.obj");
	glm::mat4 lights = glm::mat4(1.0f);





	std::vector<Model> playerObjects;
	Model character("Models/Player/bearAtlas.obj");
	playerObjects.push_back(character);
	m_Player->setPlayerModel(playerObjects);
	generateDepthMap();
	generateBloom(m_Width, m_Height);

	basic.use();
	basic.setInt("diffuse", 0);
	basic.setInt("specular", 1);

	normal.use();
	normal.setInt("normalMap", 1);
	
	bloom.use();
	bloom.setInt("diffuseTexture", 0);

	blur.use();
	blur.setInt("image", 0);

	bloomFinal.use();
	bloomFinal.setInt("scene", 0);
	bloomFinal.setInt("bloomBlur", 1);

	simpleLight.use();
	simpleLight.setInt("diffuseTexture", 0);
	simpleLight.setInt("depthMap", 1);

	//--------Loop-----------
	while (!glfwWindowShouldClose(m_Window))
	{
		float currentTime = glfwGetTime();
		timeDiff = currentTime - lastFrame;
		std::cout << timeDiff << std::endl;
		counter += timeDiff;

		if (counter >= 5.0f) {
			m_Player->setModel(false);
			counter = 0.0f;
		}
		SetFrameRateIndependency(deltaTime, lastFrame); //Frame rate independency
		//glm::vec3 playerPosition = camera.position;
		m_Player->move(m_Window, deltaTime);
		//processInput(window, deltaTime);
		SetGLFWEvents();
	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_Width, m_Height);

		/* Render background */
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f); //gray
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//------------------------SHADOWS------------------------------------------

		createCubeMapMatrix(lightPos);
		
		glViewport(0, 0, m_Width, m_Height);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		renderDepthMap(shadow, lightPos);
		renderModel(character, shadow, m_Player->getModelMatrix());
		renderModel(floor01, shadow, path01);
		renderModel(floor02, shadow, path02);
		renderModel(floor03, shadow, path03);
	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//--------------------------------------------------------------------------

		renderFloatingPoitBuffer(m_Width, m_Height);

		renderDefault(basic, m_Camera, m_Width, m_Height);
		renderModel(multipleLights, basic, lights);

		renderSimpleLight(simpleLight, m_Camera, lightPos, m_Width, m_Height, m_Shadow);
		renderModel(floor01, simpleLight, path01);
		renderModel(floor02, simpleLight, path02);
		renderModel(floor03, simpleLight, path03);
		renderModel(character, simpleLight, m_Player->getModelMatrix());

		if (m_Player->showModel)
		{
			//renderModel(cube, light, cubeObj);
			//renderModel(cube2, light, cubeObj2);
			//
			//cubeObj = glm::rotate(cubeObj, 0.01f, glm::vec3(0, 1, 0));
			//cubeObj2 = glm::rotate(cubeObj2, 0.01f, glm::vec3(0, 1, 0));
		}

		//renderDefault(simpleLight, m_Camera, lightPos, m_Width, m_Height);

		if(m_NormalMap)
		{
			renderLight(normal, m_Camera, m_Width, m_Height);
			renderModel(wall01, normal, wallMat01);
			renderModel(wall02, normal, wallMat02);
			renderModel(wall03, normal, wallMat03);
			renderModel(wall04, normal, wallMat04);
			renderModel(wall05, normal, wallMat05);
		}
		else
		{
			renderLight(pointLights, m_Camera, m_Width, m_Height);
			renderModel(wall01, pointLights, wallMat01);
			renderModel(wall02, pointLights, wallMat02);
			renderModel(wall03, pointLights, wallMat03);
			renderModel(wall04, pointLights, wallMat04);
			renderModel(wall05, pointLights, wallMat05);
		}		

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//----------BLOOM
		//bool horizontal = true, firstIt = true;
		//renderBlur(blur, horizontal, firstIt);
		//renderModel(multipleLights, blur, lights);
		//if (firstIt)
		//	firstIt = false;
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//
		//renderBloomFinal(bloomFinal, horizontal);
		//renderModel(multipleLights, bloomFinal, lights);
		//
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		std::cout << m_Player->position.x << ", " << m_Player->position.y << ", " << m_Player->position.z << std::endl;
		

		if (m_Player->position.x >= 1.0f && m_Player->position.x <= 1.3f && m_Player->position.z >= -7.0f && m_Player->position.z <= -5.0f ||
			m_Player->position.x >= -0.7f && m_Player->position.x <= -0.5f && m_Player->position.z >= -13.2f && m_Player->position.z <= -13.0f)
		{
			//lose();
		}
		
		//m_Player->getDown(deltaTime);
		//std::cout << glm::to_string(camera.position) << std::endl;
		//std::cout << glm::to_string(player.position) << std::endl;
		//glDepthFunc(GL_LESS);
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	
	}
	//clean
	basic.deleteShader();
	bloom.deleteShader();
	bloomFinal.deleteShader();
	blur.deleteShader();
	shadow.deleteShader();
	simpleLight.deleteShader();
	pointLights.deleteShader();
	normal.deleteShader();

	glfwTerminate();
}

void Application::CreateGLFWWindow()
{
	InitGLFW();
	m_Window = glfwCreateWindow(m_Width, m_Height, "Mama", NULL, NULL);

	if (!m_Window)
	{
		std::cout << "No Window was crated" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_Window);
	InitGLAD();
	SetInitialGLFWEvents();
	EnableGL();
}
















void lose()
{
	int msgboxID = MessageBox(
		NULL,
		(LPCTSTR)"Game Over! Want to try again?",
		(LPCTSTR)"Player1",
		MB_ICONQUESTION | MB_CANCELTRYCONTINUE 
	);

	switch (msgboxID)
	{
	case IDCANCEL:
		glfwTerminate();
		break;
	case IDTRYAGAIN:
		break;

	}
}