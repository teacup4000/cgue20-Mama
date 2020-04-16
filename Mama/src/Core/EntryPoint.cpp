#include "Application.h"

//This is the entry point to the application
//This is final code!
int main() {
	Application * app = new Application();
	Player* player = new Player(glm::vec3(0.0f));
	Camera* camera = new Camera(player, glm::vec3(0.0f));

	app->SetAndRun(player,camera);
	return 0;
}