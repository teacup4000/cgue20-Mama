#include "Application.h"

int main() {
	Application * app = new Application();
	Player* player = new Player(glm::vec3(0.0f));
	Camera* camera = new Camera(player, glm::vec3(0.0f));

	app->Construct(player,camera);
	//app->Run();
	return 0;
}