#include "stdafx.h"
#include "Application.h"

//#define NOMINMAX


//------globals------
int count = 0;
//-----------------------------


//render Shaders
void renderModel(Model &model, Shader &shader, glm::mat4 matrix);

void lose();

struct Cube
{
	glm::vec3 position[12] = { glm::vec3(-7.02779, 14.351f, -14.282f),
								glm::vec3(0.80503f, 11.556f, -13.148f),
								glm::vec3(4.28787f, 10.535f, -11.942f),
								glm::vec3(5.101f, 10.297f, -12.332f),
								glm::vec3(9.8748f, 9.2001f, -8.9282f),
								glm::vec3(13.1428f, 8.92818f, -10.166f),
								glm::vec3(16.609f, 8.9282f, -12.944f),
								glm::vec3(19.5601f, 8.92818, -13.144f),
								glm::vec3(19.902f, 8.9282f, -9.9726f),
								glm::vec3(9.87476f, 8.92818f, -5.80611f),
								glm::vec3(12.9748f, 8.92818f, -2.43073f),
								glm::vec3(13.3258f, 8.92818f, 0.582654f)
	};
};


void Application::Run()
{
	//--------------locals------------
	glm::vec3 lightPos = glm::vec3(7.9922f, 50.0f, -4.0616f);

	Bloom* bloom = new Bloom();
	ShadowMap* shadowMap = new ShadowMap();
	Renderer* renderer = new Renderer();
	Cube cube;

	//Frame Rate independency
	float deltaTime = 0.0f, lastFrame = 0.0f;

	//Timer variables
	float counter = 0.0f;
	float timeDiff = 0.0f;


	/*Read the settings file*/
	CreateGLFWWindow();
	renderer->Create(m_Width, m_Height, m_Camera);

	Shader basic("Shader/basic.shader");
	Shader shadow("Shader/depth.shader");
	Shader pointLights("Shader/multipleLight.shader");
	Shader normal("Shader/multipleLightsNormalMap.shader");
	Shader simpleShadow("Shader/multipleLightShadow.shader");
	Shader blur("Shader/blur.shader");
	Shader bloomFinal("Shader/bloom_final.shader");
	
	Model floor01("Models/Floor/Path01.obj");
	glm::mat4 path01 = glm::mat4(1.0f);

	Model floor02("Models/Floor/Path02.obj");
	glm::mat4 path02 = glm::mat4(1.0f);

	Model floor03("Models/Floor/Path03.obj");
	glm::mat4 path03 = glm::mat4(1.0f);

	Model floor04("Models/Floor/FloorRoom.obj");
	glm::mat4 path04 = glm::mat4(1.0f);
	
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

	Model wall06("Models/Walls/Wall06.obj");
	glm::mat4 wallMat06 = glm::mat4(1.0f);

	Model rocks("Models/Single Elements/Rock/Rocks.obj");
	glm::mat4 rockMat = glm::mat4(1.0f);

	Model woodenElements("Models/Single Elements/woodenElements.obj");
	glm::mat4 woodMat = glm::mat4(1.0f);
	
	Model multipleLights("Models/Lights/MultipleLights.obj");
	glm::mat4 lights = glm::mat4(1.0f);

	Model container("Models/Single Elements/Container/container.obj");
	glm::mat4 containerMat = glm::mat4(1.0f);

	Model debris("Models/Single Elements/Debris/debris2.obj");
	glm::mat4 debMat = glm::mat4(1.0f);

	Model cart("Models/Single Elements/MineCart/mineCart.obj");
	glm::mat4 cartMat = glm::mat4(1.0f);

	Model rails("Models/Single Elements/MineCart/rails.obj");
	glm::mat4 railMat = glm::mat4(1.0f);

	Model fence("Models/Single Elements/Fence/fence.obj");
	glm::mat4 fenceMat = glm::mat4(1.0f);

	Model boxes("Models/Single Elements/Box/boxes.obj");
	glm::mat4 boxMat = glm::mat4(1.0f);
	
	Model cubes("Models/Single Elements/Cubes/cube.obj");
	glm::mat4 cubeMat[12] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) ,glm::mat4(1.0f) ,glm::mat4(1.0f)};
	for (int i = 0; i < sizeof(cube.position)/sizeof(cube.position[0]); i++)
	{
		cubeMat[i] = glm::translate(cubeMat[i], cube.position[i]);
	}


	Model test("Models/testobj/test.obj");
	glm::mat4 testobj = glm::mat4(1.0f);
	testobj = glm::translate(testobj, glm::vec3(-7.02779, 14.351f, -14.282f));

	std::vector<Model> playerObjects;
	Model character("Models/Player/bearAtlas.obj");
	playerObjects.push_back(character);
	m_Player->setPlayerModel(playerObjects);

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
		renderModel(floor04, shadow, path04);
		renderModel(container, shadow, containerMat);
		renderModel(woodenElements, shadow, woodMat);
		renderModel(debris, shadow, debMat);
		renderModel(cart, shadow, cartMat);
		renderModel(fence, shadow, fenceMat);
		renderModel(boxes, shadow, boxMat);

	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
 		
		//--------------------------------------------------------------------------

		renderer->SetProps();
		//setProjectionViewMatrix(m_Camera, m_Width, m_Height);

		//-----------------------------------RENDER BLOOM------------------------------------------------
	
		bloom->Bind();

		renderer->renderSimpleShadow(simpleShadow, lightPos, m_Shadow, m_Farplane, shadowMap);
		renderModel(floor01, simpleShadow, path01);
		renderModel(floor02, simpleShadow, path02);
		renderModel(floor03, simpleShadow, path03);
		renderModel(floor04, simpleShadow, path04);
		renderModel(woodenElements, simpleShadow, woodMat);
		renderModel(character, simpleShadow, m_Player->getModelMatrix());
		renderModel(container, simpleShadow, containerMat);
		renderModel(debris, simpleShadow, debMat);
		renderModel(cart, simpleShadow, cartMat);
		renderModel(rails, simpleShadow, railMat);
		renderModel(fence, simpleShadow, fenceMat);
		renderModel(boxes, simpleShadow, boxMat);

		if (m_Player->showModel)
		{
			renderer->renderDefault(basic);
			for (int i = 0; i < sizeof(cube.position) / sizeof(cube.position[0]); i++)
			{
				renderModel(cubes, basic, cubeMat[i]);
				cubeMat[i] = glm::rotate(cubeMat[i], 0.05f, glm::vec3(0, 1, 0));
			}



		}

		if(m_NormalMap)
		{
			renderer->renderLight(normal);
			renderModel(wall01, normal, wallMat01);
			renderModel(wall02, normal, wallMat02);
			renderModel(wall03, normal, wallMat03);
			renderModel(wall04, normal, wallMat04);
			renderModel(wall05, normal, wallMat05);
			renderModel(wall06, normal, wallMat06);
			renderModel(rocks, normal, rockMat);
		}
		else
		{
			renderer->renderLight(pointLights);
			renderModel(wall01, pointLights, wallMat01);
			renderModel(wall02, pointLights, wallMat02);
			renderModel(wall03, pointLights, wallMat03);
			renderModel(wall04, pointLights, wallMat04);
			renderModel(wall05, pointLights, wallMat05);
			renderModel(wall06, pointLights, wallMat06);
			renderModel(rocks, pointLights, rockMat);
		}	

		renderer->renderLight(pointLights);
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
