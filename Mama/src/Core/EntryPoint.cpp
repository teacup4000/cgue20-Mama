#include "stdafx.h"
#include "Application.h"

int main() {
	Application * app = new Application();
	Player* player = new Player(glm::vec3(-9.26916f, 12.4f, -15.9479f));
	Camera* camera = new Camera(player, glm::vec3(0.0f));

	app->Init(player,camera);
	//app->Run();
	return 0;
}