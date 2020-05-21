#include "stdafx.h"
#include "Application.h"

//#define NOMINMAX


//------globals------
int count = 0;
//-----------------------------

glm::vec3 lightPos = glm::vec3(7.9922f, 50.0f, -4.0616f);

//render Shaders
void renderModel(Model model, Shader shader, glm::mat4 matrix);

//Shadow functions in ShadowMap.cpp
void generateTestFBO(unsigned int width, unsigned int height);
void postprocessTest(Shader& shader);

void renderDefault(Shader& shader, Camera* camera, GLint& width, GLint& height);

void renderSimpleLight(Shader& shader, Camera* camera, glm::vec3& lightPos, GLint& width, GLint& height, bool shadow, float far_plane, ShadowMap* map);
void renderLight(Shader& shader, Camera* camera, GLint& width, GLint& height);
void bindTestFBO(unsigned int width, unsigned int height);

void RenderQuad();
void setProjectionViewMatrix(Camera* camera, GLint width, GLint height);
void renderSimpleShadow(Shader& shader, Camera* camera, glm::vec3& lightPos, GLint& width, GLint& height, bool shadow, float far_plane, ShadowMap* map);



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
	Bloom* bloom = new Bloom();
	ShadowMap* shadowMap = new ShadowMap();

	Shader basic("Shader/basic.shader");
	Shader shadow("Shader/depth.shader");
	Shader pointLights("Shader/multipleLight.shader");
	Shader normal("Shader/multipleLightsNormalMap.shader");
	Shader simpleLight("Shader/light.shader");
	Shader blur("Shader/blur.shader");
	Shader bloomFinal("Shader/bloom_final.shader");
	Shader emission("Shader/emission.shader");
	Shader postprocess("Shader/postprocess.shader");
	Shader simpleShadow("Shader/multipleLightShadow.shader");
	
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

	Model test("Models/testobj/test.obj");
	glm::mat4 testobj = glm::mat4(1.0f);
	testobj = glm::translate(testobj, glm::vec3(-7.02779, 14.351f, -14.282f));


	std::vector<Model> playerObjects;
	Model character("Models/Player/bearAtlas.obj");
	playerObjects.push_back(character);
	m_Player->setPlayerModel(playerObjects);

	generateTestFBO(m_Width, m_Height);
	shadowMap->GenerateDepthMap(m_Nearplane, m_Farplane);
	bloom->GenerateBloomParams(m_Width, m_Height);


	basic.use();
	basic.setInt("diffuse", 0);
	basic.setInt("specular", 1);

	normal.use();
	normal.setInt("normalMap", 1);

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
		shadowMap->GenerateCubeMap(lightPos);

		glViewport(0, 0, m_Width, m_Height);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;

		shadowMap->RenderDepthMap(shadow, lightPos);
		renderModel(character, shadow, m_Player->getModelMatrix());
		renderModel(floor01, shadow, path01);
		renderModel(floor02, shadow, path02);
		renderModel(floor03, shadow, path03);
	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
 		
		//--------------------------------------------------------------------------

	
		setProjectionViewMatrix(m_Camera, m_Width, m_Height);


		//-----------------------------------RENDER BLOOM------------------------------------------------
	


		//if (m_Player->showModel)
		//{
		//	//renderModel(cube, light, cubeObj);
		//	//renderModel(cube2, light, cubeObj2);
		//	//
		//	//cubeObj = glm::rotate(cubeObj, 0.01f, glm::vec3(0, 1, 0));
		//	//cubeObj2 = glm::rotate(cubeObj2, 0.01f, glm::vec3(0, 1, 0));
		//}
		bloom->Bind();

		renderSimpleShadow(simpleShadow, m_Camera, lightPos, m_Width, m_Height, m_Shadow, m_Farplane, shadowMap);
		renderModel(floor01, simpleShadow, path01);
		renderModel(floor02, simpleShadow, path02);
		renderModel(floor03, simpleShadow, path03);
		renderModel(character, simpleShadow, m_Player->getModelMatrix());

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

		renderLight(pointLights, m_Camera, m_Width, m_Height);
		renderModel(multipleLights, pointLights, lights);
		renderModel(floor01, pointLights, path01);
		renderModel(floor02, pointLights, path02);
		renderModel(floor03, pointLights, path03);
		renderModel(character, pointLights, m_Player->getModelMatrix());

		bloom->Unbind();
		bloom->Postprocess(blur, bloomFinal);
		bloom->Unbind();

		//-------------------------------------BLOOM END---------------------------------------------------------------

	
		
		
		std::cout << m_Player->position.x << ", " << m_Player->position.y << ", " << m_Player->position.z << std::endl;
		

		if (m_Player->position.x >= 1.0f && m_Player->position.x <= 1.3f && m_Player->position.z >= -7.0f && m_Player->position.z <= -5.0f ||
			m_Player->position.x >= -0.7f && m_Player->position.x <= -0.5f && m_Player->position.z >= -13.2f && m_Player->position.z <= -13.0f)
		{
			lose();
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
	bloomFinal.deleteShader();
	blur.deleteShader();
	shadow.deleteShader();
	simpleLight.deleteShader();
	pointLights.deleteShader();
	normal.deleteShader();

	bloom->Destroy();

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

uint32_t quadVAO, quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}