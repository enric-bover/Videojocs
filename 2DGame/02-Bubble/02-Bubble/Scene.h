#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Goomba.h"
#include "Koopa.h"
#include "block_interrogante.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	int update(int deltaTime);
	void render();

private:
	void initShaders();
	void updateCamera(int deltaTime);
	void loadLevel(int lvl);
	void loadEnemies1();
	void setPlayerIniPos();

private:
	TileMap *map;
	Player *player;
	block_interrogante* interrogante;
	Goomba *goomba[20];
	Koopa* koopa;
	ShaderProgram texProgram;
	bool first;
	float currentTime;
	glm::mat4 projection;
	float cameraX;
	int level;
	int lives;
	// Text textLifes;
	bool frontalCamera;

};


#endif // _SCENE_INCLUDE

