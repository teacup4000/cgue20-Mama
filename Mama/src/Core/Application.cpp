#include "stdafx.h"
#include "Application.h"

//render Shaders
void renderModel(Model &model, Shader &shader, glm::mat4 matrix);
void renderAnimModel(AnimModel &model, Shader &shader, glm::mat4 matrix);

struct Cube
{
	glm::vec3 position[18] = {	glm::vec3(-2.077, 12.041f, -13.686f),
								glm::vec3(0.80503f, 11.556f, -13.148f),
								glm::vec3(4.28787f, 10.535f, -11.942f),
								glm::vec3(5.101f, 10.297f, -12.332f),
								glm::vec3(9.8748f, 8.92818f, -8.9282f),
								glm::vec3(13.1428f, 8.92818f, -10.166f),
								glm::vec3(16.609f, 8.9282f, -12.944f),
								glm::vec3(19.5601f, 8.92818, -13.144f),
								glm::vec3(19.902f, 8.9282f, -9.9726f),
								glm::vec3(9.87476f, 8.92818f, -5.80611f),
								glm::vec3(12.9748f, 8.92818f, -2.43073f),
								glm::vec3(13.3258f, 8.92818f, 0.582654f),
								glm::vec3(-4.4982, 12.141, -21.777),
								glm::vec3(-1.3995, 12.141, -27.784),
								glm::vec3(8.37, 8.6946, 3.2595),
								glm::vec3(1.5233, 15.512, 29.11),
								glm::vec3(-3.77218, 15.5121, 29.11),
								glm::vec3(-5.74799, 15.512, 33.383)
	};
};

struct Food
{
	glm::vec3 position[3] = {	glm::vec3(-6.67638, 12.35, -28.739),
								glm::vec3(11.589, 8.7499, -7.8574),
								glm::vec3(21.819, 8.7499, -9.007)
	};
};

void Application::Run()
{
	glm::vec3 lightPos = glm::vec3(7.9922f, 50.0f, -4.0616f);

	Bloom* bloom = new Bloom();
	ShadowMap* shadowMap = new ShadowMap();
	Renderer* renderer = new Renderer();
	Game* m_Game = new Game();
	m_Event.SetNativeGame(m_Game);
	m_Event.SetRestart();

	bool isShown = true;
	bool isOnPos = false;
	bool isPlayed = false;
	bool soundOn = true;
	bool processedEnd = false;
	bool frustum = false;

	//Timer variables
	float counter = 0.0f;
	float timeDiff = 0.0f;

	/*Read the settings file*/
	CreateGLFWWindow();
	renderer->Create(m_Width, m_Height, m_Camera);

	//--------------------------------------------------------------SET SHADERS---------------------------------------------------------------
	Shader basic("Shader/basic.shader");
	Shader shadow("Shader/depth.shader");
	Shader pointLights("Shader/multipleLight.shader");
	Shader normal("Shader/multipleLightsNormalMap.shader");
	Shader simpleShadow("Shader/multipleLightShadow.shader");
	Shader blur("Shader/blur.shader");
	Shader bloomFinal("Shader/bloom_final.shader");
	Shader bone("Shader/multipleLightShadowBones.shader");
	Shader test("Shader/Test.shader");
	//--------------------------------------------------SET SHADER PROPERTIERS-------------------------------------------------------------
	shadowMap->GenerateDepthMap(m_Nearplane, m_Farplane);
	bloom->GenerateBloomParams(m_Width, m_Height);

	basic.use();
	basic.setInt("diffuse", 0);
	basic.setInt("specular", 1);

	simpleShadow.use();
	simpleShadow.setInt("depthMap", 1);

	bone.use();
	bone.setInt("depthMap", 1);

	test.use();
	test.setInt("depthMap", 1);
	test.setInt("normalMap", 1);

	normal.use();
	normal.setInt("normalMap", 1);

	blur.use();
	blur.setInt("image", 0);

	bloomFinal.use();
	bloomFinal.setInt("scene", 0);
	bloomFinal.setInt("bloomBlur", 1);
	//----------------------------------------------------------END SHADER PROPERTIES---------------------------------------------------------
	//----------------------------------------------------------SET MODEL PROPERTIES----------------------------------------------------------

	Model loader("Models/Loader/Loader.obj");
	glm::mat4 loadMat = glm::mat4(1.0f);
	loadMat = glm::translate(loadMat, glm::vec3(STARTPOSITION_X, STARTPOSITION_Y, STARTPOSITION_Z));
	
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

	Model box01("Models/Single Elements/Box/box01.obj");
	glm::mat4 boxMat01 = glm::mat4(1.0f);
	glm::vec3 startBox01 = box01.GetPosition();
	glm::vec3 offsetBox01 = glm::vec3(1.103, 13.298, -23.711) - startBox01;

	Model box02("Models/Single Elements/Box/box02.obj");
	glm::mat4 boxMat02 = glm::mat4(1.0f);
	glm::vec3 startBox02 = box02.GetPosition();
	glm::vec3 offsetBox02 = glm::vec3(1.103, 12.429, -24.224) - startBox02;

	Model box03("Models/Single Elements/Box/box03.obj");
	glm::mat4 boxMat03 = glm::mat4(1.0f);
	glm::vec3 startBox03 = box03.GetPosition();
	glm::vec3 offsetBox03 = glm::vec3(1.103, 12.429, -23.279) - startBox03;

	Model box04("Models/Single Elements/Box/box04.obj");
	glm::mat4 boxMat04 = glm::mat4(1.0f);
	glm::vec3 startBox04 = box04.GetPosition();
	glm::vec3 offsetBox04 = glm::vec3(1.103, 12.429, -26.617) - startBox04;

	Model box05("Models/Single Elements/Box/normalCube.obj");
	glm::mat4 boxMat05 = glm::mat4(1.0f);

	Model rock01("Models/Single Elements/MoveableRocks/smallRock01.obj");
	glm::mat4 rockMat01 = glm::mat4(1.0f);
	glm::vec3 startRock01 = glm::vec3(-7.28026, 12.1433, -28.9629);
	glm::vec3 offsetRock01 = glm::vec3(-7.289, 12.151, -28.958) - startRock01;

	Model rock02("Models/Single Elements/MoveableRocks/smallRock02.obj");
	glm::mat4 rockMat02 = glm::mat4(1.0f);
	glm::vec3 startRock02 = glm::vec3(-2.25287, 12.3593, -25.8399);
	glm::vec3 offsetRock02 = glm::vec3(-2.297, 12.244, -25.744) - startRock02;

	Model rock03("Models/Single Elements/MoveableRocks/smallRock03.obj");
	glm::mat4 rockMat03 = glm::mat4(1.0f);
	glm::vec3 startRock03 = glm::vec3(-4.99139, 12.2145, -24.6432);
	glm::vec3 offsetRock03 = glm::vec3(-5.017, 12.298, -24.887) - startRock03;

	AnimModel cowboy("Animation/Cowboy/model.dae");
	glm::mat4 boyMat = glm::mat4(1.0f);
	glm::vec3 boyStartPos = glm::vec3(-0.85059, 12, -23.9644);
	float startX = -0.85059f;
	float endX = 5.91987;
	boyMat = glm::translate(boyMat, boyStartPos);
	boyMat = glm::scale(boyMat, glm::vec3(0.2f, 0.2f, 0.2f));
	boyMat = glm::rotate(boyMat, getRad(90), glm::vec3(-1, 0, 0));
	boyMat = glm::rotate(boyMat, getRad(90), glm::vec3(0, 0, -1));

	Cube* cube = new Cube();
	Model cubes("Models/Single Elements/Cubes/cube.obj");
	glm::mat4 cubeMat[18] = {	glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), 
								glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), 
								glm::mat4(1.0f), glm::mat4(1.0f) ,glm::mat4(1.0f) ,glm::mat4(1.0f),
								glm::mat4(1.0f), glm::mat4(1.0f) ,glm::mat4(1.0f) ,glm::mat4(1.0f),
								glm::mat4(1.0f) ,glm::mat4(1.0f) };
	for (int i = 0; i < sizeof(cube->position) / sizeof(cube->position[0]); i++)
		cubeMat[i] = glm::translate(cubeMat[i], cube->position[i]);

	Food* food = new Food();
	Model foods("Models/Single Elements/Food/Food.obj");
	glm::mat4 foodMat[3] = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	for (int i = 0; i < sizeof(food->position) / sizeof(food->position[0]); i++)
		foodMat[i] = glm::translate(foodMat[i], food->position[i]);

	std::vector<AnimModel> playerObjects;
	AnimModel character("Animation/Player/bear.fbx");
	playerObjects.push_back(character);
	m_Player->setPlayerModel(playerObjects);

	AnimModel mama("Animation/Mama/mama.fbx");
	glm::mat4 mamaMat = glm::mat4(1.0f);
	mamaMat = glm::translate(mamaMat, glm::vec3(-1.7344, 15.3156, 32.f));
	mamaMat = glm::scale(mamaMat, glm::vec3(1.5f, 1.5f, 1.5f));
	mamaMat = glm::rotate(mamaMat, getRad(90), glm::vec3(0, 1, 0));

	Model loseScreen("Models/GUI/gameOver.obj");
	glm::mat4 loseScreenMat = glm::mat4(1.0f);
	loseScreenMat = glm::translate(loseScreenMat, glm::vec3(-10, -10, -10));
	loseScreenMat = glm::scale(loseScreenMat, glm::vec3(3, 3, 3));

	Model winScreen("Models/GUI/Win.obj");
	glm::mat4 winScreenMat = glm::mat4(1.0f);
	winScreenMat = glm::translate(winScreenMat, glm::vec3(-10, -10, 0));
	winScreenMat = glm::scale(winScreenMat, glm::vec3(3, 3, 3));

	Model healthBar("Models/GUI/lifeBar.obj");
	glm::mat4 healthBarMat = glm::mat4(1.0f);
	//-----------------------------------------------------------END MODEL PROPERTIES---------------------------------------------------------
	//-----------------------------------------------------------SET PHYSX PROPERTIES---------------------------------------------------------

	m_PhysX->setGame(m_Game);
	m_PhysX->initPhysx();
	m_Player->m_Controller = m_PhysX->getController();
	m_Camera->setPhysx(m_PhysX);

	//invert the surfaces of wall01 and wall03 to work around wrong modelling
	for (int i = 0; i < wall01.meshes[0].indices.size(); i++) {
		if (i % 3 == 2) {
			int buffer = wall01.meshes[0].indices[i - 2];
			wall01.meshes[0].indices[i - 2] = wall01.meshes[0].indices[i];
			wall01.meshes[0].indices[i] = buffer;
		}
	}

	for (int i = 0; i < wall03.meshes[0].indices.size(); i++) {
		if (i % 3 == 2) {
			int buffer = wall03.meshes[0].indices[i - 2];
			wall03.meshes[0].indices[i - 2] = wall03.meshes[0].indices[i];
			wall03.meshes[0].indices[i] = buffer;
		}
	}
	//end inversion

	std::vector<Model> models;

	models.push_back(floor01);
	models.push_back(floor02);
	models.push_back(floor03);
	models.push_back(floor04);

	models.push_back(wall01);
	models.push_back(wall02);
	models.push_back(wall03);
	models.push_back(wall04);
	models.push_back(wall05);
	models.push_back(wall06);

	models.push_back(rocks);
	models.push_back(multipleLights);
	models.push_back(woodenElements);
	models.push_back(rails);
	models.push_back(cart);
	models.push_back(fence);
	//models.push_back(box01);
	//models.push_back(box02);
	//models.push_back(box03);
	//models.push_back(box04);


	//Collision Cubes for downloaded objects
	//models.push_back(Model("Physx/Collision/OtherElems/cartCube.obj"));
	models.push_back(Model("PhysX/Collision/OtherElems/containerCube.obj"));
	models.push_back(Model("PhysX/Collision/OtherElems/debris.obj"));

	m_PhysX->createModels(models);
	
	//m_DynamicObjects.push_back(m_PhysX->createDynamicOld(PxVec3(startRock01.x, startRock01.y, startRock01.z), Physx::DynamicType::SPHERE, PxVec3(0.1f, 0.0f, 0.0f), 2));
	//m_PhysX->createDynamic(PxVec3(startRock02.x, startRock02.y, startRock02.z), Physx::DynamicType::CAPSULE, PxVec3(0.15f, 0.2f, 0.0f), 2);

	m_DynamicObjects.push_back(m_PhysX->createDynamic(rock01, 2));
	m_DynamicObjects.push_back(m_PhysX->createDynamic(rock02, 3));
	m_DynamicObjects.push_back(m_PhysX->createDynamic(rock03, 10));
	m_DynamicObjects.push_back(m_PhysX->createDynamic(box01, 1));
	m_DynamicObjects.push_back(m_PhysX->createDynamic(box02, 1));
	m_DynamicObjects.push_back(m_PhysX->createDynamic(box03, 1));
	m_DynamicObjects.push_back(m_PhysX->createDynamic(box04, 1));


	for (int i = 0; i < sizeof(cube->position) / sizeof(cube->position[0]); i++) {
		m_PhysX->createTrigger(PxVec3(cube->position[i].x, cube->position[i].y, cube->position[i].z), PxVec3(0.2f, 0.4f, 0.2f), Physx::TriggerType::TRAP);
	}
	for (int i = 0; i < sizeof(food->position) / sizeof(food->position[0]); i++) {
		m_PhysX->createTrigger(PxVec3(food->position[i].x, food->position[i].y, food->position[i].z), PxVec3(0.3f, 0.3f, 0.3f), Physx::TriggerType::MEAT);
	}
	m_PhysX->createTrigger(PxVec3(-1.7344, 15.3156, 32.f), PxVec3(1.0f, 0.5f, 0.5f), Physx::TriggerType::MOMMY);

	//------------------------------------------------------------END PHYSX PROPERTIES---------------------------------------------------------
	//------------------------------------------------------------SET SOUND PROPERTIES---------------------------------------------------------
	IrrKlang* sound = new IrrKlang();
	sound->createSound();
	sound->play("Assets/sounds/hazy-cosmos.mp3", true);

	IrrKlang* snore = new IrrKlang();
	snore->createSound();

	IrrKlang* win = new IrrKlang();
	win->createSound();

	IrrKlang* lose = new IrrKlang();
	lose->createSound();
	//-------------------------------------------------------------------END SOUND-------------------------------------------------------------

	cart.GetSortedVertices();

	while (!glfwWindowShouldClose(m_Window))
	{
		if (m_Event.isVsync()) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		if (m_Event.isRestart()) {
			
			//reset application variables
			counter = 0.0f;
			processedEnd = false;
			
			//reset player and CCT positions
			m_Player->Reset();
			//reset camera position and direction
			m_Camera->Reset();
			//reset collision and triggers
			m_PhysX->Reset();
			
			//create new game (full health, unpaused, DEFAULT mode) and forward it to event and physx
			m_Game = new Game();
			m_Event.SetNativeGame(m_Game);
			m_PhysX->setGame(m_Game);

			//reset cowboy position
			isOnPos = false;
			boyStartPos = glm::vec3(-0.85059, 12, -23.9644);
			boyMat = glm::mat4(1.0f);
			boyMat = glm::translate(boyMat, boyStartPos);
			boyMat = glm::scale(boyMat, glm::vec3(0.2f, 0.2f, 0.2f));
			boyMat = glm::rotate(boyMat, getRad(90), glm::vec3(-1, 0, 0));
			boyMat = glm::rotate(boyMat, getRad(90), glm::vec3(0, 0, -1));

			//reset Music
			sound->stop();
			sound->play("Assets/sounds/hazy-cosmos.mp3", true);

			//deactivate restart
			m_Event.SetRestart();
		}

		SetFrameRateIndependency(); //Frame rate independency
		if (!m_Game->isPaused()) {
			if (m_Event.isCameraControl()) {
				m_Camera->move(m_Window, m_DeltaTime);
			}
			else {
				m_Player->move(m_Window, m_DeltaTime);
			}
			m_PhysX->simulate(m_DeltaTime);
		}
		SetGLFWEvents();
		
		if (m_Player->getPosition().y < 4.0f) {
			m_Game->Lose();
		}
	
		//--------------------------------------------------------------RENDER SHADOWS------------------------------------------------------#	
		renderer->SetProps(m_Brightness);
		shadowMap->GenerateCubeMap(lightPos);

		glClearColor(0.5f, 0.5f, 0.5f, 0.0f); //gray
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;

		shadowMap->RenderDepthMap(shadow, lightPos);

		if (renderer->isFrustum(floor01, path01, m_Event.isFrustum()))
			renderModel(floor01, shadow, path01);

		if (renderer->isFrustum(floor02, path02, m_Event.isFrustum()))
			renderModel(floor02, shadow, path02);

		if (renderer->isFrustum(floor03, path03, m_Event.isFrustum()))
			renderModel(floor03, shadow, path03);

		if (renderer->isFrustum(floor04, path04, m_Event.isFrustum()))
			renderModel(floor04, shadow, path04);

		if (renderer->isFrustum(container, containerMat, m_Event.isFrustum()))
			renderModel(container, shadow, containerMat);

		if (renderer->isFrustum(woodenElements, woodMat, m_Event.isFrustum()))
			renderModel(woodenElements, shadow, woodMat);

		if (renderer->isFrustum(debris, debMat, m_Event.isFrustum()))
			renderModel(debris, shadow, debMat);

		if (renderer->isFrustum(cart, cartMat, m_Event.isFrustum()))
		{
			frustum = true;
			renderModel(cart, shadow, cartMat);
		}
		std::cout << "mineCart = " << frustum << std::endl;
		std::cout << "Distance = " << cart.GetDistance() << std::endl;
		std::cout << "MinPos = " << cart.GetMinPos().x << ", " << cart.GetMinPos().y << ", " << cart.GetMinPos().z << std::endl;
		std::cout << "MaxPos = " << cart.GetMaxPos().x << ", " << cart.GetMaxPos().y << ", " << cart.GetMaxPos().z << std::endl;
		std::cout << "Center = " << cart.GetCenter().x << ", " << cart.GetCenter().y << ", " << cart.GetCenter().z << std::endl;


		if (renderer->isFrustum(fence, fenceMat, m_Event.isFrustum()))
			renderModel(fence, shadow, fenceMat);

		if (renderer->isFrustum(cart, cartMat, m_Event.isFrustum()))
			renderModel(cart, shadow, cartMat);

		if (renderer->isFrustum(fence, fenceMat, m_Event.isFrustum()))
			renderModel(fence, shadow, fenceMat);

		if (renderer->isFrustum(box01, boxMat01, m_Event.isFrustum()))
			renderModel(box01, shadow, boxMat01);

		if (renderer->isFrustum(box02, boxMat02, m_Event.isFrustum()))
			renderModel(box02, shadow, boxMat02);

		if (renderer->isFrustum(box03, boxMat03, m_Event.isFrustum()))
			renderModel(box03, shadow, boxMat03);

		if (renderer->isFrustum(box04, boxMat04, m_Event.isFrustum()))
			renderModel(box04, shadow, boxMat04);

		if (!m_Game->isPaused()) {
			mama.InitShader(shadow);
			renderAnimModel(mama, shadow, mamaMat);

			character.InitShader(shadow);
			renderAnimModel(character, shadow, m_Player->getModelMatrix());

			cowboy.InitShader(shadow);
			renderAnimModel(cowboy, shadow, boyMat);
		}
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glViewport(0, 0, m_Width, m_Height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-------------------------------------------------------------SHADOWS END------------------------------------------------------------
		//-------------------------------------------------------------RENDER BLOOM-----------------------------------------------------------
		bloom->Bind();
		
		if (m_Event.isWireFrame()) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		renderer->renderDefault(basic);
		if (m_Game->getStatus() == GameStatus::LOSE || m_Event.isRenderAll()) 
			renderModel(loseScreen, basic, loseScreenMat);

		if (m_Game->getStatus() == GameStatus::WIN || m_Event.isRenderAll())
			renderModel(winScreen, basic, winScreenMat);


		//Models with shadows
		renderer->renderSimpleShadow(simpleShadow, lightPos, m_Shadow, m_Farplane, shadowMap);

		if (renderer->isFrustum(floor01, path01, m_Event.isFrustum()))
			renderModel(floor01, simpleShadow, path01);

		if (renderer->isFrustum(floor02, path02, m_Event.isFrustum()))
			renderModel(floor02, simpleShadow, path02);

		if (renderer->isFrustum(floor03, path03, m_Event.isFrustum()))
			renderModel(floor03, simpleShadow, path03);

		if (renderer->isFrustum(floor04, path04, m_Event.isFrustum()))
			renderModel(floor04, simpleShadow, path04);

		if (renderer->isFrustum(woodenElements, woodMat, m_Event.isFrustum()))
			renderModel(woodenElements, simpleShadow, woodMat);

		if (renderer->isFrustum(container, containerMat, m_Event.isFrustum()))
			renderModel(container, simpleShadow, containerMat);

		if (renderer->isFrustum(debris, debMat, m_Event.isFrustum()))
			renderModel(debris, simpleShadow, debMat);

		if (renderer->isFrustum(cart, cartMat, m_Event.isFrustum()))
			renderModel(cart, simpleShadow, cartMat);

		if (renderer->isFrustum(rails, railMat, m_Event.isFrustum()))
			renderModel(rails, simpleShadow, railMat);

		if (renderer->isFrustum(fence, fenceMat, m_Event.isFrustum()))
			renderModel(fence, simpleShadow, fenceMat);

		renderer->renderSimpleShadow(test, lightPos, m_Shadow, m_Farplane, shadowMap);
		if (renderer->isFrustum(box01, boxMat01, m_Event.isFrustum())) {
			//boxMat01 = getOrientationFromPos(m_DynamicObjects[3], startBox01);
			renderModel(box01, test, boxMat01);
		}

		if (renderer->isFrustum(box02, boxMat02, m_Event.isFrustum())) {
			//boxMat02 = getOrientationFromPos(m_DynamicObjects[4], startBox02);
			renderModel(box02, test, boxMat02);
		}

		if (renderer->isFrustum(box03, boxMat03, m_Event.isFrustum())) {
			//boxMat03 = getOrientationFromPos(m_DynamicObjects[5], startBox03);
			renderModel(box03, test, boxMat03);
		}

		if (renderer->isFrustum(box04, boxMat04, m_Event.isFrustum())) {
			boxMat04 = getOrientationFromPos(m_DynamicObjects[6], startBox04, offsetBox04);
			renderModel(box04, test, boxMat04);
		}
	

		//Models with bones and shadows
		renderer->renderSimpleShadow(bone, lightPos, m_Shadow, m_Farplane, shadowMap);
		if (!m_Game->isPaused()) {
			
			
			character.InitShader(bone);
			renderAnimModel(character, bone, m_Player->getModelMatrix());

			mama.InitShader(bone);
			renderAnimModel(mama, bone, mamaMat);

			cowboy.InitShader(bone);
			renderAnimModel(cowboy, bone, boyMat);

		}
		//Models without shadows
		renderer->renderDefault(basic);
		if (m_Player->m_ShowModel || m_Event.isRenderAll())
		{
			for (int i = 0; i < sizeof(cube->position) / sizeof(cube->position[0]); i++)
			{
				renderModel(cubes, basic, cubeMat[i]);
				if (!m_Game->isPaused()) {
					cubeMat[i] = glm::rotate(cubeMat[i], 3.0f * m_DeltaTime, glm::vec3(0, 1, 1));
				}
			}
			if (!m_Game->isPaused()) {
				counter += m_DeltaTime;
			}
			if (counter >= 5)
			{
				m_Player->m_ShowModel = false;
				counter = 0.0f;
			}
		}
		for (int i = 0; i < sizeof(food->position) / sizeof(food->position[0]); i++)
		{
			glm::vec3 pos(food->position[i]);
			if (!m_PhysX->checkMeat(PxVec3(pos.x, pos.y, pos.z)) || m_Event.isRenderAll()) {
				renderModel(foods, basic, foodMat[i]);
				if (!m_Game->isPaused()) {
					foodMat[i] = glm::rotate(foodMat[i], 3.0f * m_DeltaTime, glm::vec3(0, 1, 0));
				}
			}
		}

		if (m_NormalMap)
		{
			//Models with normal mapping
			renderer->renderLight(normal);
			if (renderer->isFrustum(wall01, wallMat01, m_Event.isFrustum()))
				renderModel(wall01, normal, wallMat01);

			if (renderer->isFrustum(wall02, wallMat02, m_Event.isFrustum()))
				renderModel(wall02, normal, wallMat02);

			if (renderer->isFrustum(wall03, wallMat03, m_Event.isFrustum()))
				renderModel(wall03, normal, wallMat03);

			if (renderer->isFrustum(wall04, wallMat04, m_Event.isFrustum()))
				renderModel(wall04, normal, wallMat04);

			if (isShown = renderer->isFrustum(wall05, wallMat05, m_Event.isFrustum()))
				renderModel(wall05, normal, wallMat05);

			if (renderer->isFrustum(wall06, wallMat06, m_Event.isFrustum()))
				renderModel(wall06, normal, wallMat06);

			if (renderer->isFrustum(rocks, rockMat, m_Event.isFrustum()))
				renderModel(rocks, normal, rockMat);

			if (renderer->isFrustum(box05, boxMat05, m_Event.isFrustum())) {
				//boxMat04 = getOrientationFromPos(m_DynamicObjects[6], startBox04, offsetBox04);
				renderModel(box05, test, boxMat05);
			}
			
		}
		else
		{
			//models with multiple lights
			renderer->renderLight(pointLights);
			if (renderer->isFrustum(wall01, wallMat01, m_Event.isFrustum()))
				renderModel(wall01, pointLights, wallMat01);

			if (renderer->isFrustum(wall02, wallMat02, m_Event.isFrustum()))
				renderModel(wall02, pointLights, wallMat02);

			if (renderer->isFrustum(wall03, wallMat03, m_Event.isFrustum()))
				renderModel(wall03, pointLights, wallMat03);

			if (renderer->isFrustum(wall04, wallMat04, m_Event.isFrustum()))
				renderModel(wall04, pointLights, wallMat04);

			if (renderer->isFrustum(wall05, wallMat05, m_Event.isFrustum()))
				renderModel(wall05, pointLights, wallMat05);

			if (renderer->isFrustum(wall06, wallMat06, m_Event.isFrustum()))
				renderModel(wall06, pointLights, wallMat06);

			if (renderer->isFrustum(rocks, rockMat, m_Event.isFrustum()))
				renderModel(rocks, pointLights, rockMat);

			if (renderer->isFrustum(rock01, rockMat01, m_Event.isFrustum())) {
				//rockMat01 = getOrientationFromPos(m_DynamicObjects[0], startRock01);
				renderModel(rock01, pointLights, rockMat01);
			}

			if (renderer->isFrustum(rock02, rockMat02, m_Event.isFrustum())) {
				//rockMat02 = getOrientationFromPos(m_DynamicObjects[1], startRock02);
				renderModel(rock02, pointLights, rockMat02);
			}

			if (renderer->isFrustum(rock03, rockMat03, m_Event.isFrustum())) {
				//rockMat03 = getOrientationFromPos(m_DynamicObjects[2], startRock03);
				renderModel(rock03, pointLights, rockMat03);
			}

			if (renderer->isFrustum(box01, boxMat01, m_Event.isFrustum())) {
				//boxMat01 = getOrientationFromPos(m_DynamicObjects[3], startBox01);
				renderModel(box01, pointLights, boxMat01);
			}

			if (renderer->isFrustum(box02, boxMat02, m_Event.isFrustum())){
				//boxMat02 = getOrientationFromPos(m_DynamicObjects[4], startBox02);
				renderModel(box02, pointLights, boxMat02);
			}

			if (renderer->isFrustum(box03, boxMat03, m_Event.isFrustum())) {
				//boxMat03 = getOrientationFromPos(m_DynamicObjects[5], startBox03);
				renderModel(box03, pointLights, boxMat03);
			}

			if (renderer->isFrustum(box04, boxMat04, m_Event.isFrustum())) {
				boxMat04 = getOrientationFromPos(m_DynamicObjects[6], startBox04, offsetBox04);
				renderModel(box04, pointLights, boxMat04);
			}
		}

		renderer->renderLight(pointLights);
		if (renderer->isFrustum(multipleLights, lights, m_Event.isFrustum()))
			renderModel(multipleLights, pointLights, lights);
		if (renderer->isFrustum(rock01, rockMat01, m_Event.isFrustum())) {
			//rockMat01 = getOrientationFromPos(m_DynamicObjects[0], startRock01);
			renderModel(rock01, pointLights, rockMat01);
		}

		if (renderer->isFrustum(rock02, rockMat02, m_Event.isFrustum())) {
			//rockMat02 = getOrientationFromPos(m_DynamicObjects[1], startRock02);
			renderModel(rock02, pointLights, rockMat02);
		}

		if (renderer->isFrustum(rock03, rockMat03, m_Event.isFrustum())) {
			//rockMat03 = getOrientationFromPos(m_DynamicObjects[2], startRock03);
			renderModel(rock03, pointLights, rockMat03);
		}
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		bloom->Unbind();
		bloom->Postprocess(blur, bloomFinal);
		bloom->Unbind();

		//glFlush();
		//---------------------------------------------------------------BLOOM END--------------------------------------------------------------
		glViewport(0, 0, m_Width, m_Height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (m_Game->getStatus() == GameStatus::DEFAULT || m_Event.isRenderAll()) {
			healthBarMat = m_Camera->getHealthBarMat(m_Game->getLife() / 100.0f);
			//healthBarMat = glm::scale(healthBarMat, glm::vec3(0.1, 0.1, 0.1));
			renderModel(healthBar, basic, healthBarMat);
		}

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		glFlush();
		
		//----------------------------------------------------------------WIN/LOSE--------------------------------------------------------------
		if (m_Game->getStatus() == GameStatus::LOSE && !processedEnd) {
			if (!m_Event.isMuted()) {
			sound->stop();
			lose->play("Assets/sounds/lose.mp3", false);
			lose->setVolume(m_Event.getVolume());
		}
			m_Game->pauseGame();
			m_Camera->loseScreen();
			processedEnd = true;
		}
		if (m_Game->getStatus() == GameStatus::WIN && !processedEnd) {
			if (!m_Event.isMuted()) {
				sound->stop();
				snore->stop();
				win->play("Assets/sounds/win.mp3", false);
				win->setVolume(m_Event.getVolume());
			}
			m_Game->pauseGame();
			m_Camera->winScreen();
			processedEnd = true;
		}
		//----------------------------------------------------------------END WIN/LOSE----------------------------------------------------------
		//---------------------------------------------------------SET ADDITIONAL SOUNDS---------------------------------------------------------
		if (!m_Event.isMuted()) {
			sound->setVolume(m_Event.getVolume());
			if (!soundOn) {
				sound->play("Assets/sounds/hazy-cosmos.mp3", true);
				soundOn = true;
			}

			if (!m_Game->isPaused()) {
				if (m_Player->getPosition().y >= 13 && m_Player->getPosition().z >= 22)
				{
					if (!isPlayed)
					{
						snore->play("Assets/sounds/snore.mp3", true);
						snore->setVolume(0.1 * m_Event.getVolume());
					}
					isPlayed = true;
				}
				if (m_Player->getPosition().y > 14)
				{
					if (isPlayed)
						snore->setVolume(0.8 * m_Event.getVolume());
				}
			}
			if (m_Player->getPosition().y < 13 && snore != 0 || m_Game->isPaused() && snore != 0)
			{
				snore->stop();
				isPlayed = false;
			}
		}
		else {
			sound->stop();
			soundOn = false;
			if (isPlayed) {
				snore->stop();
				isPlayed = false;
			}
		}
		//---------------------------------------------------------------END SOUNDS--------------------------------------------------------------
		//-----------------------------------------------------COWBOY RUNNING FUNCTION-----------------------------------------------------------
		if (!m_Game->isPaused()) {
			if (boyStartPos.x < -0.3f) {
				m_PhysX->createTrigger(PxVec3(0.3f, boyStartPos.y, boyStartPos.z), PxVec3(0.1f, 0.9f, 0.3f), Physx::TriggerType::BOY);
			}
			else if (boyStartPos.x > 4.0f) {
				m_PhysX->createTrigger(PxVec3(-4.0f, boyStartPos.y, boyStartPos.z), PxVec3(0.1f, 0.9f, 0.1f), Physx::TriggerType::BOY);
			}
			else {
				m_PhysX->createTrigger(PxVec3(-boyStartPos.x, boyStartPos.y, boyStartPos.z), PxVec3(0.1f, 0.9f, 0.3f), Physx::TriggerType::BOY);
			}
			if (boyStartPos.x < endX && !isOnPos && m_DeltaTime < 1.0f)
			{
				boyMat = glm::translate(boyMat, glm::vec3(0, -20 * m_DeltaTime, 0)); //-0.4
				boyStartPos.x += 10 * m_DeltaTime;
				if (boyStartPos.x > endX)
				{
					boyMat = glm::rotate(boyMat, getRad(180), glm::vec3(0, 0, 1));
					isOnPos = !isOnPos;	
				}
			}
			else if (boyStartPos.x > startX && isOnPos && m_DeltaTime < 1.0f)
			{
				boyMat = glm::translate(boyMat, glm::vec3(0, -20 * m_DeltaTime, 0));
				boyStartPos.x -= 10 * m_DeltaTime;
				if (boyStartPos.x < startX)
				{
					boyMat = glm::rotate(boyMat, getRad(180), glm::vec3(0, 0, -1));
					isOnPos = !isOnPos;
				}
			}
		}
		//---------------------------------------------------------------------------------------------------------------------------------------
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
	m_PhysX->releasePhysx();
	glfwTerminate();
}

glm::mat4 Application::getOrientationFromPos(PxRigidDynamic* actor, glm::vec3 startPos, glm::vec3 offset) {
	
	PxTransform pos = actor->getGlobalPose();
	PxTransform mPos = actor->getCMassLocalPose();
	char buff[1000];
	sprintf(buff, "phys: %f %f %f\n", mPos.p.x, mPos.p.y, mPos.p.z);
	OutputDebugString(buff);
	sprintf(buff, "graf: %f %f %f\n", mPos.p.x - offset.x, mPos.p.y - offset.y, mPos.p.z - offset.z);
	//OutputDebugString(buff);
	sprintf(buff, "off: %f %f %f\n", offset.x, offset.y, offset.z);
	//OutputDebugString(buff);
	sprintf(buff, "start: %f %f %f\n", startPos.x, startPos.y, startPos.z);
	//OutputDebugString(buff);
	//sprintf(buff, "%f %f %f - %f\n", pos.q.x, pos.q.y, pos.q.z, pos.q.w);
	//OutputDebugString(buff);
		
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(mPos.p.x - offset.x, mPos.p.y - offset.y, mPos.p.z - offset.z));
	//mat = glm::translate(mat, glm::vec3(pos.p.x, pos.p.y, pos.p.z));
	//mat = glm::translate(mat, glm::vec3(startPos.x, startPos.y, startPos.z));
	
	if (pos.q.x != 0 && pos.q.y != 0 && pos.q.z != 0) {
		float a = PxAcos(pos.q.w);
		float s = PxSin(a);
		//mat = glm::rotate(mat, a / 0.5f, glm::vec3(pos.q.x / s, pos.q.y / s, pos.q.z / s));
	}

	mat = glm::translate(mat, glm::vec3(-startPos.x, -startPos.y, -startPos.z));
	
	return mat;
}

void Application::CreateGLFWWindow()
{
	InitGLFW();
	if (m_Fullscreen)
		m_Window = glfwCreateWindow(m_Width, m_Height, "Mama", glfwGetPrimaryMonitor(), NULL);
	else
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