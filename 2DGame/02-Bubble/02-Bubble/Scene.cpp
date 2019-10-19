#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

// --
#define SCREEN_X 0
#define SCREEN_Y 0
// --
#define INIT_PLAYER_X_TILES 3
#define INIT_PLAYER_Y_TILES 11

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

// --
void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	goomba = new Goomba();
	goomba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	goomba->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 3) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	// --
	cameraX = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	goomba->update(deltaTime);
	// --
	updateCamera(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	projection = glm::ortho(cameraX, cameraX + float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	//+++
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	goomba->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

// --
void Scene::updateCamera(int deltaTime)
{
	float playerX = float(player->getPositionX());
	if (playerX < SCREEN_WIDTH / 5)
		cameraX = 0;
	else if (playerX < map->getMapWidth() - 3*SCREEN_WIDTH / 5)
	{
		if (playerX - cameraX < SCREEN_WIDTH / 5)
			cameraX = playerX - SCREEN_WIDTH / 5;
		if (playerX - cameraX > 2 * SCREEN_WIDTH / 5)
			cameraX = playerX - 2 * SCREEN_WIDTH / 5;
	}
}