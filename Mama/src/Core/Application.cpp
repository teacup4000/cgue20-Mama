#include "stdafx.h"
#include "Application.h"




//------globals------
int count = 0;
//-----------------------------


//render Shaders
void renderModel(Model &model, Shader &shader, glm::mat4 matrix);

//void lose();

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
	Game* game = new Game();
	Cube cube;
	event.SetRestart();

	bool isShown = true;

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
	float posX = floor01.GetPosition().x, poxY = floor01.GetPosition().y, posZ = floor01.GetPosition().z;
	std::cout << "floor radius = " << floor01.GetDistance()/4 << std::endl;
	std::cout << "floor position = x: " << posX << ", y: " << poxY << ", z. " << posZ << std::endl;
	std::cout << "floor min = " << floor01.GetMin().x <<", " << floor01.GetMin().y << ", " << floor01.GetMin().z << ", max = " << floor01.GetMax().x << ", "<< floor01.GetMax().y << ", " << floor01.GetMax().z  << std::endl;

	Model floor02("Models/Floor/Path02.obj");
	glm::mat4 path02 = glm::mat4(1.0f);
	std::cout << "floor radius = " << floor02.GetDistance() / 2 << std::endl;
	//std::cout << "floor position = x: " << floor02.GetPosition().x << ", y: " << floor02.GetPosition.directory << ", z. " << floor02.GetPosition().z << std::endl;

	Model floor03("Models/Floor/Path03.obj");
	glm::mat4 path03 = glm::mat4(1.0f);
	std::cout << "floor radius = " << floor03.GetDistance() / 2 << std::endl;
	//std::cout << "floor position = x: " << floor03.GetPosition().x << ", y: " << floor03.GetPosition.directory << ", z. " << floor03.GetPosition().z << std::endl;

	Model floor04("Models/Floor/FloorRoom.obj");
	glm::mat4 path04 = glm::mat4(1.0f);
	std::cout << "floor radius = " << floor04.GetDistance() / 2 << std::endl;
	//std::cout << "floor position = x: " << floor04.GetPosition().x << ", y: " << floor04.GetPosition.directory << ", z. " << floor04.GetPosition().z << std::endl;

	Model wall01("Models/Walls/Wall01.obj");
	glm::mat4 wallMat01 = glm::mat4(1.0f);
	std::cout << "wall radius = " << wall01.GetDistance() / 2 << std::endl;
	//std::cout << "wall position = x: " << wall01.GetPosition().x << ", y: " << wall01.GetPosition.directory << ", z. " << wall01.GetPosition().z << std::endl;

	Model wall02("Models/Walls/Wall02.obj");
	glm::mat4 wallMat02 = glm::mat4(1.0f);
	std::cout << "wall radius = " << wall02.GetDistance() / 2 << std::endl;
	//std::cout << "wall position = x: " << wall02.GetPosition().x << ", y: " << wall02.GetPosition.directory << ", z. " << wall02.GetPosition().z << std::endl;

	Model wall03("Models/Walls/Wall03.obj");
	glm::mat4 wallMat03 = glm::mat4(1.0f);
	std::cout << "wall radius = " << wall03.GetDistance() / 2 << std::endl;
	//std::cout << "wall position = x: " << wall03.GetPosition().x << ", y: " << wall03.GetPosition.directory << ", z. " << wall03.GetPosition().z << std::endl;

	Model wall04("Models/Walls/Wall04.obj");
	glm::mat4 wallMat04 = glm::mat4(1.0f);
	std::cout << "wall radius = " << wall04.GetDistance() / 2 << std::endl;
	//std::cout << "wall position = x: " << wall04.GetPosition().x << ", y: " << wall04.GetPosition.directory << ", z. " << wall04.GetPosition().z << std::endl;

	Model wall05("Models/Walls/Wall05.obj");
	glm::mat4 wallMat05 = glm::mat4(1.0f);
	std::cout << "wall radius = " << wall05.GetDistance() / 2 << std::endl;
	//std::cout << "wall position = x: " << wall05.GetPosition().x << ", y: " << wall05.GetPosition.directory << ", z. " << wall05.GetPosition().z << std::endl;

	Model wall06("Models/Walls/Wall06.obj");
	glm::mat4 wallMat06 = glm::mat4(1.0f);
	std::cout << "wall radius = " << wall06.GetDistance() / 2 << std::endl;
	//std::cout << "wall position = x: " << wall06.GetPosition().x << ", y: " << wall06.GetPosition.directory << ", z. " << wall06.GetPosition().z << std::endl;

	Model rocks("Models/Single Elements/Rock/Rocks.obj");
	glm::mat4 rockMat = glm::mat4(1.0f);
	std::cout << "rock radius = " << rocks.GetDistance() / 2 << std::endl;
	//std::cout << "rock position = x: " << rocks.GetPosition().x << ", y: " << rocks.GetPosition.directory << ", z. " << rocks.GetPosition().z << std::endl;

	Model woodenElements("Models/Single Elements/woodenElements.obj");
	glm::mat4 woodMat = glm::mat4(1.0f);
	std::cout << "wood radius = " << woodenElements.GetDistance() / 2 << std::endl;
	//std::cout << "wood position =  x: " << woodenElements.GetPosition().x << ", y: " << woodenElements.GetPosition.directory << ", z. " << woodenElements.GetPosition().z << std::endl;

	Model multipleLights("Models/Lights/MultipleLights.obj");
	glm::mat4 lights = glm::mat4(1.0f);
	std::cout << "lights radius = " << multipleLights.GetDistance() / 2 << std::endl;
	//std::cout << "lights position = x: " << multipleLights.GetPosition().x << ", y: " << multipleLights.GetPosition.directory << ", z. " << multipleLights.GetPosition().z << std::endl;

	Model container("Models/Single Elements/Container/container.obj");
	glm::mat4 containerMat = glm::mat4(1.0f);
	std::cout << "container radius = " << container.GetDistance() / 2 << std::endl;
	//std::cout << "container position = x: " << container.GetPosition().x << ", y: " << floor01.GetPosition.directory << ", z. " << floor01.GetPosition().z << std::endl;

	Model debris("Models/Single Elements/Debris/debris2.obj");
	glm::mat4 debMat = glm::mat4(1.0f);
	std::cout << "debris radius = " << debris.GetDistance() / 2 << std::endl;
	//std::cout << "debris position = x: " << debris.GetPosition().x << ", y: " << debris.GetPosition.directory << ", z. " << debris.GetPosition().z << std::endl;

	Model cart("Models/Single Elements/MineCart/mineCart.obj");
	glm::mat4 cartMat = glm::mat4(1.0f);
	std::cout << "cart radius = " << cart.GetDistance() / 2 << std::endl;
	//std::cout << "cart position = x: " << cart.GetPosition().x << ", y: " << cart.GetPosition.directory << ", z. " << cart.GetPosition().z << std::endl;

	Model rails("Models/Single Elements/MineCart/rails.obj");
	glm::mat4 railMat = glm::mat4(1.0f);
	std::cout << "rails radius = " << rails.GetDistance() / 2 << std::endl;
	//std::cout << "rails position = x: " << rails.GetPosition().x << ", y: " << rails.GetPosition.directory << ", z. " << rails.GetPosition().z << std::endl;

	Model fence("Models/Single Elements/Fence/fence.obj");
	glm::mat4 fenceMat = glm::mat4(1.0f);
	std::cout << "fence radius = " << fence.GetDistance() / 2 << std::endl;
	//std::cout << "fence position = x: " << fence.GetPosition().x << ", y: " << fence.GetPosition.directory << ", z. " << fence.GetPosition().z << std::endl;

	Model boxes("Models/Single Elements/Box/boxes.obj");
	glm::mat4 boxMat = glm::mat4(1.0f);
	std::cout << "boxes radius = " << boxes.GetDistance() / 2 << std::endl;
	//std::cout << "boxes position = x: " << boxes.GetPosition().x << ", y: " << boxes.GetPosition.directory << ", z. " << boxes.GetPosition().z << std::endl;

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

		if (renderer->isFrustum(floor01, path01))
			renderModel(floor01, shadow, path01);

		if (renderer->isFrustum(floor02, path02))
			renderModel(floor02, shadow, path02);

		if (renderer->isFrustum(floor03, path03))
			renderModel(floor03, shadow, path03);

		if (renderer->isFrustum(floor04, path04))
			renderModel(floor04, shadow, path04);

		if (renderer->isFrustum(container, containerMat))
		{
			renderModel(container, shadow, containerMat);
		}

		if (renderer->isFrustum(woodenElements, woodMat))
			renderModel(woodenElements, shadow, woodMat);

		if (renderer->isFrustum(debris, debMat))
			renderModel(debris, shadow, debMat);

		if (renderer->isFrustum(cart, cartMat))
			renderModel(cart, shadow, cartMat);

		if (renderer->isFrustum(fence, fenceMat))
			renderModel(fence, shadow, fenceMat);

		if (renderer->isFrustum(boxes, boxMat))
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
		if (renderer->isFrustum(floor01, path01))
			renderModel(floor01, simpleShadow, path01);
		
		if (renderer->isFrustum(floor02, path02))
			renderModel(floor02, simpleShadow, path02);
		
		if (renderer->isFrustum(floor03, path03))
			renderModel(floor03, simpleShadow, path03);
		
		if (renderer->isFrustum(floor04, path04))
			renderModel(floor04, simpleShadow, path04);

		if (renderer->isFrustum(woodenElements, woodMat))
			renderModel(woodenElements, simpleShadow, woodMat);

		if (renderer->isFrustum(container, containerMat))
			renderModel(container, simpleShadow, containerMat);

		if (renderer->isFrustum(debris, debMat))
			renderModel(debris, simpleShadow, debMat);

		if (renderer->isFrustum(cart, cartMat))
			renderModel(cart, simpleShadow, cartMat);

		if (renderer->isFrustum(rails, railMat))
			renderModel(rails, simpleShadow, railMat);

		if (renderer->isFrustum(fence, fenceMat))
			renderModel(fence, simpleShadow, fenceMat);

		if (renderer->isFrustum(boxes, boxMat))
			renderModel(boxes, simpleShadow, boxMat);

		renderModel(character, simpleShadow, m_Player->getModelMatrix());

		if (m_Player->m_ShowModel)
		{
			renderer->renderDefault(basic);
			for (int i = 0; i < sizeof(cube.position) / sizeof(cube.position[0]); i++)
			{
				if (renderer->isFrustum(cubes, cubeMat[i]))
					renderModel(cubes, basic, cubeMat[i]);
				cubeMat[i] = glm::rotate(cubeMat[i], 0.05f, glm::vec3(0, 1, 0));
			}
		}

		if(m_NormalMap)
		{
			renderer->renderLight(normal);
			if (renderer->isFrustum(wall01, wallMat01))
				renderModel(wall01, normal, wallMat01);

			if (renderer->isFrustum(wall02, wallMat02))
				renderModel(wall02, normal, wallMat02);

			if (renderer->isFrustum(wall03, wallMat03))
				renderModel(wall03, normal, wallMat03);

			if (renderer->isFrustum(wall04, wallMat04))
				renderModel(wall04, normal, wallMat04);

			if (isShown = renderer->isFrustum(wall05, wallMat05))
				renderModel(wall05, normal, wallMat05);

			if (isShown)
				std::cout << "is shown is " << isShown << std::endl;
			if (!isShown)
				std::cout << "is shown is " << isShown << std::endl;


			if (renderer->isFrustum(wall06, wallMat06))
				renderModel(wall06, normal, wallMat06);

			if (renderer->isFrustum(rocks, rockMat))
				renderModel(rocks, normal, rockMat);
		}
		else
		{
			renderer->renderLight(pointLights);
			if (renderer->isFrustum(wall01, wallMat01))
				renderModel(wall01, pointLights, wallMat01);

			if (renderer->isFrustum(wall02, wallMat02))
				renderModel(wall02, pointLights, wallMat02);

			if (renderer->isFrustum(wall03, wallMat03))
				renderModel(wall03, pointLights, wallMat03);

			if (renderer->isFrustum(wall04, wallMat04))
				renderModel(wall04, pointLights, wallMat04);

			if (renderer->isFrustum(wall05, wallMat05))
				renderModel(wall05, pointLights, wallMat05);

			if (renderer->isFrustum(wall06, wallMat06))
				renderModel(wall06, pointLights, wallMat06);

			if (renderer->isFrustum(rocks, rockMat))
				renderModel(rocks, pointLights, rockMat);
		}	

		renderer->renderLight(pointLights);

		if (renderer->isFrustum(multipleLights, lights))
			renderModel(multipleLights, pointLights, lights);

		bloom->Unbind();
		bloom->Postprocess(blur, bloomFinal);
		bloom->Unbind();

		//-------------------------------------BLOOM END---------------------------------------------------------------

		std::cout << m_Player->getPlayerPosition().x << ", " << m_Player->getPlayerPosition().y << ", " << m_Player->getPlayerPosition().z << std::endl;
		
		for (int i = 0; i < sizeof(cube.position) / sizeof(cube.position[0]); i++) {
			//ToDo: If Player collides with cube objects
			if (m_Player->getPlayerPosition() == cube.position[i]) {
				//lose
			}
		}

		//
		//if (m_Player->getPlayerPosition().x >= 1.0f && m_Player->getPlayerPosition().x <= 1.3f && m_Player->getPlayerPosition().z >= -7.0f && m_Player->getPlayerPosition().z <= -5.0f ||
		//	m_Player->getPlayerPosition().x >= -0.7f && m_Player->getPlayerPosition().x <= -0.5f && m_Player->getPlayerPosition().z >= -13.2f && m_Player->getPlayerPosition().z <= -13.0f)
		//{
		//	//lose();
		//}
		
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





//void lose()
//{
//	int msgboxID = MessageBox(
//		NULL,
//		(LPCTSTR)"Game Over! Want to try again?",
//		(LPCTSTR)"Player1",
//		MB_ICONQUESTION | MB_CANCELTRYCONTINUE 
//	);
//
//	switch (msgboxID)
//	{
//	case IDCANCEL:
//		glfwTerminate();
//		break;
//	case IDTRYAGAIN:
//		break;
//	}
//}
