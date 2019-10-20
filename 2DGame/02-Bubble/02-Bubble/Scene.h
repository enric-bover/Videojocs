#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Goomba.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void updateCamera(int deltaTime);

private:
	TileMap *map;
	Player *player;
	Goomba *goomba[20];
	ShaderProgram texProgram;
	bool first;
	float currentTime;
	glm::mat4 projection;

	// --
	float cameraX;

};


#endif // _SCENE_INCLUDE

