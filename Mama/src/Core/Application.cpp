#include "stdafx.h"
#include "Application.h"
#include "PxPhysicsAPI.h"
using namespace physx;



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

	//Bloom* bloom = new Bloom();
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
	//Shader blur("Shader/blur.shader");
	//Shader bloomFinal("Shader/bloom_final.shader");
	
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
	//bloom->GenerateBloomParams(m_Width, m_Height);

	basic.use();
	basic.setInt("diffuse", 0);
	basic.setInt("specular", 1);

	normal.use();
	normal.setInt("normalMap", 1);

	//blur.use();
	//blur.setInt("image", 0);
	//
	//bloomFinal.use();
	//bloomFinal.setInt("scene", 0);
	//bloomFinal.setInt("bloomBlur", 1);

	//--------setup Physx-------
	physx->initPhysx();

	m_Player->controller = physx->getController();
	
	std::vector<Model> models;

	models.push_back(floor01);
	models.push_back(floor02);
	//models.push_back(floor03);
	models.push_back(floor04);
	//Collision Walls
	models.push_back(Model("Models/CollisionCubes/cube01.obj"));
	models.push_back(Model("Models/CollisionCubes/cube02.obj"));
	models.push_back(Model("Models/CollisionCubes/cube03.obj"));
	models.push_back(Model("Models/CollisionCubes/cube04.obj"));
	models.push_back(Model("Models/CollisionCubes/cube05.obj"));
	models.push_back(Model("Models/CollisionCubes/cube06.obj"));
	models.push_back(Model("Models/CollisionCubes/cube07.obj"));
	models.push_back(Model("Models/CollisionCubes/cube08.obj"));
	models.push_back(Model("Models/CollisionCubes/cube09.obj"));
	models.push_back(Model("Models/CollisionCubes/cube10.obj")); 
	models.push_back(Model("Models/CollisionCubes/cube11.obj"));
	models.push_back(Model("Models/CollisionCubes/cube12.obj"));
	models.push_back(Model("Models/CollisionCubes/cube13.obj"));
	models.push_back(Model("Models/CollisionCubes/cube14.obj"));
	models.push_back(Model("Models/CollisionCubes/cube15.obj"));
	models.push_back(Model("Models/CollisionCubes/cube16.obj"));
	models.push_back(Model("Models/CollisionCubes/cube17.obj"));
	models.push_back(Model("Models/CollisionCubes/cube18.obj"));
	models.push_back(Model("Models/CollisionCubes/cube19.obj"));
	models.push_back(Model("Models/CollisionCubes/cube20.obj")); 
	models.push_back(Model("Models/CollisionCubes/cube21.obj"));
	models.push_back(Model("Models/CollisionCubes/cube22.obj"));
	models.push_back(Model("Models/CollisionCubes/cube23.obj"));
	models.push_back(Model("Models/CollisionCubes/cube24.obj"));
	models.push_back(Model("Models/CollisionCubes/cube25.obj"));
	models.push_back(Model("Models/CollisionCubes/cube26.obj"));
	models.push_back(Model("Models/CollisionCubes/cube27.obj"));
	models.push_back(Model("Models/CollisionCubes/cube28.obj"));
	models.push_back(Model("Models/CollisionCubes/cube29.obj"));
	models.push_back(Model("Models/CollisionCubes/cube30.obj"));
	models.push_back(Model("Models/CollisionCubes/cube31.obj"));
	models.push_back(Model("Models/CollisionCubes/cube32.obj"));
	models.push_back(Model("Models/CollisionCubes/cube33.obj"));
	models.push_back(Model("Models/CollisionCubes/cube34.obj"));
	models.push_back(Model("Models/CollisionCubes/cube35.obj"));
	models.push_back(Model("Models/CollisionCubes/cube36.obj"));
	models.push_back(Model("Models/CollisionCubes/cube37.obj"));
	models.push_back(Model("Models/CollisionCubes/cube38.obj"));
	models.push_back(Model("Models/CollisionCubes/cube39.obj"));
	models.push_back(Model("Models/CollisionCubes/cube40.obj")); 
	models.push_back(Model("Models/CollisionCubes/cube41.obj"));
	models.push_back(Model("Models/CollisionCubes/cube42.obj"));
	models.push_back(Model("Models/CollisionCubes/cube43.obj"));
	models.push_back(Model("Models/CollisionCubes/cube44.obj"));
	models.push_back(Model("Models/CollisionCubes/cube45.obj"));
	models.push_back(Model("Models/CollisionCubes/cube46.obj"));
	models.push_back(Model("Models/CollisionCubes/cube47.obj"));
	models.push_back(Model("Models/CollisionCubes/cube48.obj"));
	models.push_back(Model("Models/CollisionCubes/cube49.obj"));
	models.push_back(Model("Models/CollisionCubes/cube50.obj"));
	models.push_back(Model("Models/CollisionCubes/cube51.obj"));
	models.push_back(Model("Models/CollisionCubes/cube52.obj"));
	models.push_back(Model("Models/CollisionCubes/cube53.obj"));
	models.push_back(Model("Models/CollisionCubes/cube54.obj"));
	models.push_back(Model("Models/CollisionCubes/cube55.obj"));
	models.push_back(Model("Models/CollisionCubes/cube56.obj"));
	models.push_back(Model("Models/CollisionCubes/cube57.obj"));
	models.push_back(Model("Models/CollisionCubes/cube58.obj"));
	models.push_back(Model("Models/CollisionCubes/cube59.obj"));
	models.push_back(Model("Models/CollisionCubes/cube60.obj"));
	models.push_back(Model("Models/CollisionCubes/cube61.obj"));
	models.push_back(Model("Models/CollisionCubes/cube62.obj"));
	models.push_back(Model("Models/CollisionCubes/cube63.obj"));
	models.push_back(Model("Models/CollisionCubes/cube64.obj"));
	models.push_back(Model("Models/CollisionCubes/cube65.obj"));
	models.push_back(Model("Models/CollisionCubes/cube66.obj"));
	models.push_back(Model("Models/CollisionCubes/cube67.obj"));
	models.push_back(Model("Models/CollisionCubes/cube68.obj"));
	models.push_back(Model("Models/CollisionCubes/cube69.obj"));
	models.push_back(Model("Models/CollisionCubes/cube70.obj"));
	models.push_back(Model("Models/CollisionCubes/cube71.obj"));
	//Collision FLoors
	models.push_back(Model("Models/CollisionCubes/cube72.obj"));
	models.push_back(Model("Models/CollisionCubes/cube73.obj"));
	models.push_back(Model("Models/CollisionCubes/cube74.obj"));
	models.push_back(Model("Models/CollisionCubes/cube75.obj"));
	models.push_back(Model("Models/CollisionCubes/cube76.obj"));
	models.push_back(Model("Models/CollisionCubes/cube77.obj"));
	models.push_back(Model("Models/CollisionCubes/cube78.obj"));
	models.push_back(Model("Models/CollisionCubes/cube79.obj"));



	// TODO den convex bs mit triangle meshes fixen(or not?)
	/*models.push_back(wall01);
	models.push_back(wall02);
	models.push_back(wall03);
	models.push_back(wall04);
	models.push_back(wall05);
	models.push_back(wall06);*/

	physx->createModels(models);

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
		
		//simulate Physx
		physx->simulate();
	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_Width, m_Height);

		/* Render background */
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f); //gray
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//------------------------SHADOWS------------------------------------------
		//shadowMap->GenerateCubeMap(lightPos);
		//
		//glViewport(0, 0, m_Width, m_Height);
		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	std::cout << "Framebuffer not complete" << std::endl;
		//
		//shadowMap->RenderDepthMap(shadow, lightPos);
		//renderModel(character, shadow, m_Player->getModelMatrix());
		//
		//if (renderer->isFrustum(floor01, path01))
		//	renderModel(floor01, shadow, path01);
		//
		//if (renderer->isFrustum(floor02, path02))
		//	renderModel(floor02, shadow, path02);
		//
		//if (renderer->isFrustum(floor03, path03))
		//	renderModel(floor03, shadow, path03);
		//
		//if (renderer->isFrustum(floor04, path04))
		//	renderModel(floor04, shadow, path04);
		//
		//if (renderer->isFrustum(container, containerMat))
		//	renderModel(container, shadow, containerMat);
		//
		//if (renderer->isFrustum(woodenElements, woodMat))
		//	renderModel(woodenElements, shadow, woodMat);
		//
		//if (renderer->isFrustum(debris, debMat))
		//	renderModel(debris, shadow, debMat);
		//
		//if (renderer->isFrustum(cart, cartMat))
		//	renderModel(cart, shadow, cartMat);
		//
		//if (renderer->isFrustum(fence, fenceMat))
		//	renderModel(fence, shadow, fenceMat);
		//
		//if (renderer->isFrustum(boxes, boxMat))
		//	renderModel(boxes, shadow, boxMat);
		//
		//
		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	std::cout << "Framebuffer not complete" << std::endl;
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
 		//
		//--------------------------------------------------------------------------
		//glClearColor(0,0,0,0); //gray
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer->SetProps();

		//setProjectionViewMatrix(m_Camera, m_Width, m_Height);

		//-----------------------------------RENDER BLOOM------------------------------------------------
	
		//bloom->Bind();
		//--------------------THIS IS TEMPORARY----------------------------------------------------------
		renderer->renderDefault(basic);
		{
			renderModel(floor01, basic, path01);
			renderModel(floor02, basic, path02);
			renderModel(floor03, basic, path03);
			renderModel(floor04, basic, path04);
			renderModel(woodenElements, basic, woodMat);
			renderModel(container, basic, containerMat);
			renderModel(woodenElements, basic, woodMat);
			renderModel(container, basic, containerMat);
			renderModel(debris, basic, debMat);
			renderModel(cart, basic, cartMat);
			renderModel(rails, basic, railMat);
			renderModel(fence, basic, fenceMat);
			renderModel(boxes, basic, boxMat);
			renderModel(character, basic, m_Player->getModelMatrix());
			renderModel(wall01, basic, wallMat01);
			renderModel(wall02, basic, wallMat02);
			renderModel(wall03, basic, wallMat03);
			renderModel(wall04, basic, wallMat04);
			renderModel(wall05, basic, wallMat05);
			renderModel(wall06, basic, wallMat06);
		}
		//-----------------------END TEMPORARY-------------------------------------------------------

		//renderer->renderSimpleShadow(simpleShadow, lightPos, m_Shadow, m_Farplane, shadowMap);
		//if (renderer->isFrustum(floor01, path01))
		//	renderModel(floor01, simpleShadow, path01);
		//
		//if (renderer->isFrustum(floor02, path02))
		//	renderModel(floor02, simpleShadow, path02);
		//
		//if (renderer->isFrustum(floor03, path03))
		//	renderModel(floor03, simpleShadow, path03);
		//
		//if (renderer->isFrustum(floor04, path04))
		//	renderModel(floor04, simpleShadow, path04);
		//
		//if (renderer->isFrustum(woodenElements, woodMat))
		//	renderModel(woodenElements, simpleShadow, woodMat);
		//
		//if (renderer->isFrustum(container, containerMat))
		//	renderModel(container, simpleShadow, containerMat);
		//
		//if (renderer->isFrustum(debris, debMat))
		//	renderModel(debris, simpleShadow, debMat);
		//
		//if (renderer->isFrustum(cart, cartMat))
		//	renderModel(cart, simpleShadow, cartMat);
		//
		//if (renderer->isFrustum(rails, railMat))
		//	renderModel(rails, simpleShadow, railMat);
		//
		//if (renderer->isFrustum(fence, fenceMat))
		//	renderModel(fence, simpleShadow, fenceMat);
		//
		//if (renderer->isFrustum(boxes, boxMat))
		//	renderModel(boxes, simpleShadow, boxMat);
		//
		//renderModel(character, simpleShadow, m_Player->getModelMatrix());
		//
		//if (m_Player->m_ShowModel)
		//{
		//	renderer->renderDefault(basic);
		//	for (int i = 0; i < sizeof(cube.position) / sizeof(cube.position[0]); i++)
		//	{
		//		if (renderer->isFrustum(cubes, cubeMat[i]))
		//			renderModel(cubes, basic, cubeMat[i]);
		//		cubeMat[i] = glm::rotate(cubeMat[i], 0.05f, glm::vec3(0, 1, 0));
		//	}
		//}
		//
		//if(m_NormalMap)
		//{
		//	renderer->renderLight(normal);
		//	if (renderer->isFrustum(wall01, wallMat01))
		//		renderModel(wall01, normal, wallMat01);
		//
		//	if (renderer->isFrustum(wall02, wallMat02))
		//		renderModel(wall02, normal, wallMat02);
		//
		//	if (renderer->isFrustum(wall03, wallMat03))
		//		renderModel(wall03, normal, wallMat03);
		//
		//	if (renderer->isFrustum(wall04, wallMat04))
		//		renderModel(wall04, normal, wallMat04);
		//
		//	if (isShown = renderer->isFrustum(wall05, wallMat05))
		//		renderModel(wall05, normal, wallMat05);
		//
		//	if (isShown)
		//		std::cout << "is shown is " << isShown << std::endl;
		//	if (!isShown)
		//		std::cout << "is shown is " << isShown << std::endl;
		//
		//
		//	if (renderer->isFrustum(wall06, wallMat06))
		//		renderModel(wall06, normal, wallMat06);
		//
		//	if (renderer->isFrustum(rocks, rockMat))
		//		renderModel(rocks, normal, rockMat);
		//}
		//else
		//{
		//	renderer->renderLight(pointLights);
		//	if (renderer->isFrustum(wall01, wallMat01))
		//		renderModel(wall01, pointLights, wallMat01);
		//
		//	if (renderer->isFrustum(wall02, wallMat02))
		//		renderModel(wall02, pointLights, wallMat02);
		//
		//	if (renderer->isFrustum(wall03, wallMat03))
		//		renderModel(wall03, pointLights, wallMat03);
		//
		//	if (renderer->isFrustum(wall04, wallMat04))
		//		renderModel(wall04, pointLights, wallMat04);
		//
		//	if (renderer->isFrustum(wall05, wallMat05))
		//		renderModel(wall05, pointLights, wallMat05);
		//
		//	if (renderer->isFrustum(wall06, wallMat06))
		//		renderModel(wall06, pointLights, wallMat06);
		//
		//	if (renderer->isFrustum(rocks, rockMat))
		//		renderModel(rocks, pointLights, rockMat);
		//}	
		//
		//renderer->renderLight(pointLights);
		//
		//if (renderer->isFrustum(multipleLights, lights))
		//	renderModel(multipleLights, pointLights, lights);

		//bloom->Unbind();
		//bloom->Postprocess(blur, bloomFinal);
		//bloom->Unbind();

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
	//bloomFinal.deleteShader();
	//blur.deleteShader();
	shadow.deleteShader();
	pointLights.deleteShader();
	normal.deleteShader();

	//bloom->Destroy();

	glfwTerminate();

	physx->releasePhysx();
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
