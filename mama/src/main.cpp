#define STB_IMAGE_IMPLEMENTATION
#define NOMINMAX
//STB_Image is a file loading library that is important for loading JPG and PNG files.

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#include <cstdio>
#include <ctime>
#include <Windows.h>

#include "Shader.h"
#include "Camera.h"
#include "Player.h"
#include "INIReader.h"

#include <iostream>

#include <GLFW\glfw3.h>


//------globals------
//Windows properties
GLFWwindow* window;
GLint windowWidth;
GLint windowHeight;
extern float lastX = (float)(windowWidth / 2.0);
extern float lastY = (float)(windowHeight / 2.0);
int count = 0;
//-----------------------------


Player player(glm::vec3(0, 0, 0));
Camera camera(&player, glm::vec3(10, 0, 103));
glm::vec3 lightPos = glm::vec3(0.0, 20.0f, 0.0f);

//----------functions-------------
int createWindow(int& width, int&height);
//framerate independency in initFunction.cpp
void computeTimeLogic(float& deltaTime, float& lastFrame);

//Camera function in Camera.cpp
//void processInput(GLFWwindow* window, float& deltaTime);

//all init functions in initFunctions.cpp
void initGlfw();
void initGlad();
void enableGL();
void initMouse(GLFWwindow* window);
void initReader(GLint& width, GLint& height, float& brightness);

//Key and Mouse input functions in Camera.cpp
void key_callback(GLFWwindow* window, int key, int code, int action, int modifers);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

//render Shaders
void LightShaderSetup(Shader& shader, Camera& camera, GLint& width, GLint& height, float& brightness);
void renderModel(Model model, Shader shader, glm::mat4 matrix);

//Shadow functions in ShadowMap.cpp
void generateDepthMap();
void createCubeMapMatrix(Camera& camera, glm::vec3& lightPos);
void renderDepthMap(Shader& shader, glm::vec3& lightPos);
void renderNormal(Shader& shader, Camera& camera, glm::vec3& lightPos, GLint& width, GLint& height);

void lose();

int main()
{
	//--------------locals------------
	//window properties
	float brightness = 0.0f;

	//Frame Rate independency
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//Timer variables
	float counter = 0.0f;
	float timeDiff = 0.0f;

	/*Read the settings file*/
	initReader(windowWidth, windowHeight, brightness);
	createWindow(windowWidth, windowHeight);

	Shader light("Shader/basic.shader");
	Shader depthShader("Shaders/depth.shader");


	//Model test("Models/test.obj");
	//glm::mat4 testObj = glm::mat4(1.0f);

	Model floor("Models/FloorAtlas.obj");
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
	cubeObj2 = glm::translate(cubeObj2, glm::vec3(-8.0f, 3.0f, -130.3f));
	
	/*create the PlayerObjects*/
	std::vector<Model> playerObjects;
	Model character("Models/bearAtlas.obj");
	playerObjects.push_back(character);
	player.setPlayerModel(playerObjects);

	//generateDepthMap();

	light.use();
	light.setInt("material.diffuse2", 0);

	

	//--------Loop-----------
	while (!glfwWindowShouldClose(window))
	{
		
		float currentTime = glfwGetTime();
		timeDiff = currentTime - lastFrame;
		std::cout << timeDiff << std::endl;
		counter += timeDiff;

		if (counter >= 5.0f) {
			player.setModel(false);
			counter = 0.0f;
		}
		computeTimeLogic(deltaTime, lastFrame); //Frame rate independency
		//glm::vec3 playerPosition = camera.position;
		player.move(window, deltaTime);
		//processInput(window, deltaTime);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glViewport(0, 0, windowWidth, windowHeight);

		/* Render background */
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f); //gray
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//createCubeMapMatrix(camera, lightPos);




		LightShaderSetup(light, camera, windowWidth, windowHeight, brightness);
		{
			light.use();
			//renderModel(test, basic, testObj);

			renderModel(floor, light, floorObj);
			renderModel(wall, light, wallObj);
			renderModel(mountain, light, mountainObj);
			if (player.showModel)
			{
				renderModel(cube, light, cubeObj);
				renderModel(cube2, light, cubeObj2);

				cubeObj = glm::rotate(cubeObj, 0.01f, glm::vec3(0,1,0));			
				cubeObj2 = glm::rotate(cubeObj2, 0.01f, glm::vec3(0, 1, 0));
			}

			for (Model model : player.getPlayerobject()) {
				renderModel(character, light, player.getModelMatrix());
			}
		}
		if (player.position.x >= 1.0f && player.position.x <= 1.3f && player.position.z >= -7.0f && player.position.z <= -5.0f ||
			player.position.x >= -0.7f && player.position.x <= -0.5f && player.position.z >= -13.2f && player.position.z <= -13.0f)
		{
			lose();
		}
		
		
		//std::cout << glm::to_string(camera.position) << std::endl;
		//std::cout << glm::to_string(player.position) << std::endl;
		glDepthFunc(GL_LESS);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//clean
	light.deleteShader();
	glfwTerminate();
	return 0;
}

int createWindow(int& width, int& height)
{
	initGlfw();
	window = glfwCreateWindow(width, height, "Mama", NULL, NULL);

	if (!window)
	{
		std::cout << "No Window was crated" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	initGlad();
	initMouse(window);
	enableGL();
	return 0;
}

void setFullscreen(bool& fullscreen)
{
	if(fullscreen)
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, windowWidth, windowHeight, 0); /*Fullscreen always uses the Primary Monitor! */
	else
		glfwSetWindowMonitor(window, NULL, 0, 0, windowWidth, windowHeight, 0);
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