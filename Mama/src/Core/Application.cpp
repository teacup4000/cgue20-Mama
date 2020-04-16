#include "Application.h"

#define NOMINMAX
#include <iostream>
#include <cstdio>
#include <ctime>

//Define a light Position
glm::vec3 LightPos = glm::vec3(0.0, 10.0f, 0.0f);

//Render a Model
//function called in Model.cpp
void renderModel(Model model, Shader shader, glm::mat4 matrix);

//Define Shadow Mapping
//functions in Renderer.cpp
void generateDepthMap();
void createCubeMapMatrix(glm::vec3& lightPos);

//Render a Model with the depth.shader
void renderDepthMap(Shader& shader, glm::vec3& lightPos);

//Render a Model with a normal light.shader 
void renderNormal(Shader& shader, Camera* camera, glm::vec3& lightPos, GLint& width, GLint& height);

//Win/Lose condition
void lose();

void Application::Run()
{
	//--------------------------------SET SOME LOCAL VARIABLES------------------------------------------------
	//Frame Rate independency
	float deltaTime = 0.0f, lastFrame = 0.0f;

	//Timer variables
	float counter = 0.0f;
	float timeDiff = 0.0f;

	//--------------------------------------------------------------------------------------------------------
	CreateGLFWWindow();

	//Shader basic("Shader/basic.shader");
	Shader light("Shader/light.shader");
	Shader depthShader("Shader/depth.shader");

	//----------------------------------------SIMPLE OBJECTS FOR TESTING--------------------------------------
									//Model test1("Models/testobj/floor.obj");
									//glm::mat4 testFloorObj = glm::mat4(1.0f);
									//
									//Model test2("Models/testobj/wall.obj");
									//glm::mat4 testWallObj = glm::mat4(1.0f);
	//--------------------------------------------------------------------------------------------------------

	//--------------------------------------------LOAD MODELS FROM FILE---------------------------------------
	//ENVIRONMENT
		Model floor("Models/FloorAtlas.obj");
		glm::mat4 floorObj = glm::mat4(1.0f);
	
		Model wall("Models/wallAtlas.obj");
		glm::mat4 wallObj = glm::mat4(1.0f);
	
		Model mountain("Models/MountainAtlas.obj");
		glm::mat4 mountainObj = glm::mat4(1.0f);
	
	//CUBES YOU SHOULDNT RUN INTO
		Model cube("Models/cube.obj");
		glm::mat4 cubeObj = glm::mat4(1.0f);
		cubeObj = glm::scale(cubeObj, glm::vec3(0.1f, 0.1f, 0.1f));
		cubeObj = glm::translate(cubeObj, glm::vec3(10.0f, 3.0f, -60.3f));
	
		Model cube2("Models/cube2.obj");
		glm::mat4 cubeObj2 = glm::mat4(1.0f);
		cubeObj2 = glm::scale(cubeObj2, glm::vec3(0.1f, 0.1f, 0.1f));
		cubeObj2 = glm::translate(cubeObj2, glm::vec3(-8.0f, 3.0f, -130.3f));

	//PLAYER OBJECT
		std::vector<Model> playerObjects;
		Model character("Models/bearAtlas.obj");
		playerObjects.push_back(character);
		m_Player->setPlayerModel(playerObjects);

	
	generateDepthMap();

	light.use();
	light.setInt("diffuseTexture", 0);
	light.setInt("depthMap", 1);

	depthShader.use();

	//-----------------------------------------BEGIN GAME LOOP------------------------------------------------
	while (!glfwWindowShouldClose(m_Window))
	{
		//CHARACTER JUMP
			float currentTime = glfwGetTime(); //this should be merged with frame rate independency
			timeDiff = currentTime - lastFrame;
			counter += timeDiff;

			if (counter >= 5.0f) {				//this should be defined inside a private function
				m_Player->setModel(false);
				counter = 0.0f;
			}

		SetFrameRateIndependency(deltaTime, lastFrame); 
		m_Player->move(m_Window, deltaTime);
		SetGLFWEvents();

		//BACKGROUND COLOR
			glClearColor(0.5f, 0.5f, 0.5f, 0.0f); //gray
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//--------------------------------------START SHADOW MAPPING----------------------------------------
		createCubeMapMatrix(LightPos);

		glViewport(0, 0, m_Width, m_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		depthShader.use();

		//FIRST: RENDER THE DEPTH MAP
			renderDepthMap(depthShader, LightPos);
		//THEN RENDER ALL MODELD THAT SHOULD CONTAIN SHADOWS
		{
			//renderModel(test1, depthShader, testFloorObj);
			//renderModel(test2, depthShader, testWallObj);
			renderModel(floor, depthShader, floorObj);
			renderModel(wall, depthShader, wallObj);
			renderModel(mountain, depthShader, mountainObj);

			for (Model model : m_Player->getPlayerobject()) {
				renderModel(character, depthShader, m_Player->getModelMatrix());
			}
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//-----------------------------------------END SHADOW MAPPING----------------------------------------
		//-------------------------------------------BEGIN RENDERING-----------------------------------------
		//SECOND: RENDER THE NORMAL MAP
			renderNormal(light, m_Camera, LightPos, m_Width, m_Height);
		//THEN RENDER ALL MODELS THAT SHOULD BE SHOWN
		{
			//renderModel(test1, light, testFloorObj);
			//renderModel(test2, light, testWallObj);
			renderModel(floor, light, floorObj);
			renderModel(wall, light, wallObj);
			renderModel(mountain, light, mountainObj);
			if (m_Player->showModel)
			{
				renderModel(cube, light, cubeObj);
				renderModel(cube2, light, cubeObj2);

				cubeObj = glm::rotate(cubeObj, 0.01f, glm::vec3(0, 1, 0));
				cubeObj2 = glm::rotate(cubeObj2, 0.01f, glm::vec3(0, 1, 0));
			}

			for (Model model : m_Player->getPlayerobject()) {
				renderModel(character, light, m_Player->getModelMatrix());
			}
		}

		//------------------------------------------------END RENDERING---------------------------------------
		//WIN/LOSE CONDITION
		if (m_Player->position.x >= 1.0f && m_Player->position.x <= 1.3f && m_Player->position.z >= -7.0f && m_Player->position.z <= -5.0f ||
			m_Player->position.x >= -0.7f && m_Player->position.x <= -0.5f && m_Player->position.z >= -13.2f && m_Player->position.z <= -13.0f)
		{
			lose();
		}
		
		//IF THE PLAYER JUMPS HE SHOULD GET DOWN AGAIN
		m_Player->getDown(deltaTime);

		//SWAP AND POLL
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	
	}
	//CLEAN
	light.deleteShader();
	depthShader.deleteShader();
	glfwTerminate();
}

//Create the GLFW Window
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
	//INITIALIZE IMPORTANT GLFW FUNCTIONS AND EVENTS
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