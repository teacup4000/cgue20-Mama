#include "Application.h"

//#define NOMINMAX

#include <cstdio>
#include <ctime>

#include <iostream>

//------globals------
int count = 0;
//-----------------------------

glm::vec3 lightPos = glm::vec3(0.0, 10.0f, 0.0f);

//render Shaders
void renderModel(Model model, Shader shader, glm::mat4 matrix);

//Shadow functions in ShadowMap.cpp
void generateDepthMap();
void createCubeMapMatrix(glm::vec3& lightPos);
void renderDepthMap(Shader& shader, glm::vec3& lightPos);
void renderNormal(Shader& shader, Camera* camera, glm::vec3& lightPos, GLint& width, GLint& height);

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

	//Shader basic("Shader/basic.shader");
	Shader light("Shader/light.shader");
	Shader depthShader("Shader/depth.shader");

	//Create Test Objects
	Model test1("Models/testobj/floor.obj");
	glm::mat4 testFloorObj = glm::mat4(1.0f);

	Model test2("Models/testobj/wall.obj");
	glm::mat4 testWallObj = glm::mat4(1.0f);

	//Other objects are to the right
																																				/*Model floor("Models/FloorAtlas.obj");
																																							glm::mat4 floorObj = glm::mat4(1.0f);

																																							Model wall("Models/wallAtlas.obj");
																																							glm::mat4 wallObj = glm::mat4(1.0f);

																																							Model mountain("Models/MountainAtlas.obj");
																																							glm::mat4 mountainObj = glm::mat4(1.0f);

																																							Model cube("Models/cube.obj");
																																							glm::mat4 cubeObj = glm::mat4(1.0f);
																																							cubeObj = glm::scale(cubeObj, glm::vec3(0.1f, 0.1f, 0.1f));
																																							cubeObj = glm::translate(cubeObj, glm::vec3(10.0f, 3.0f, -60.3f));

																																							Model cube2("Models/cube2.obj");
																																							glm::mat4 cubeObj2 = glm::mat4(1.0f);
																																							cubeObj2 = glm::scale(cubeObj2, glm::vec3(0.1f, 0.1f, 0.1f));
																																							cubeObj2 = glm::translate(cubeObj2, glm::vec3(-8.0f, 3.0f, -130.3f));*/
	
	/*create the PlayerObjects*/
																																								/*
																																								std::vector<Model> playerObjects;
																																								Model character("Models/bearAtlas.obj");
																																								playerObjects.push_back(character);
																																								player.setPlayerModel(playerObjects);*/
	generateDepthMap();

	light.use();
	light.setInt("diffuseTexture", 0);
	light.setInt("depthMap", 1);

	depthShader.use();

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

		/*glViewport(0, 0, windowWidth, windowHeight);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		depthShader.use();*/

		renderDepthMap(depthShader, lightPos);
		renderModel(test1, depthShader, testFloorObj);
		renderModel(test2, depthShader, testWallObj);
		/*renderModel(floor, depthShader, floorObj);
		renderModel(wall, depthShader, wallObj);
		renderModel(mountain, depthShader, mountainObj);

		for (Model model : player.getPlayerobject()) {
			renderModel(character, depthShader, player.getModelMatrix());
		}*/
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//--------------------------------------------------------------------------
		renderNormal(light, m_Camera, lightPos, m_Width, m_Height);
		renderModel(test1, light, testFloorObj);
		renderModel(test2, light, testWallObj);

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
		/*renderModel(floor, basic, floorObj);
		renderModel(wall, basic, wallObj);
		renderModel(mountain, basic, mountainObj);
		if (player.showModel)
		{
			renderModel(cube, light, cubeObj);
			renderModel(cube2, light, cubeObj2);

			cubeObj = glm::rotate(cubeObj, 0.01f, glm::vec3(0, 1, 0));
			cubeObj2 = glm::rotate(cubeObj2, 0.01f, glm::vec3(0, 1, 0));
		}

		for (Model model : player.getPlayerobject()) {
			renderModel(character, basic, player.getModelMatrix());
		}*/

		

		/*if (player.position.x >= 1.0f && player.position.x <= 1.3f && player.position.z >= -7.0f && player.position.z <= -5.0f ||
			player.position.x >= -0.7f && player.position.x <= -0.5f && player.position.z >= -13.2f && player.position.z <= -13.0f)
		{
			lose();
		}
		
		player.getDown(deltaTime);*/
		//std::cout << glm::to_string(camera.position) << std::endl;
		//std::cout << glm::to_string(player.position) << std::endl;
		//aglDepthFunc(GL_LESS);
	
	}
	//clean
	light.deleteShader();
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