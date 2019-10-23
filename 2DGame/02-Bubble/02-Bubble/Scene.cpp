#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

// --
#define CAMERA_X 0
#define CAMERA_Y 0
#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240
#define NUMBER_OF_GOOMBAS 20
// --
#define INIT_PLAYER_X_TILES 3
#define INIT_PLAYER_Y_TILES 10

Scene::Scene()
{
	map = NULL;
	player = NULL;
	first = true;
	level = 1;
	lives = 5;
	frontalCamera = false;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

void Scene::init()
{
	initShaders();
	player = new Player();
	player->init(glm::ivec2(CAMERA_X, CAMERA_Y), texProgram);
	loadLevel(level);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	cameraX = 0.0f;

	/*// Select which font you want to use
	if (!textLives.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;*/
}

int Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	bool isDead = false;
	for (int i = 0; i < 1 && !isDead; i++) {
		if (goomba[i]->getPositionX() < cameraX + float(CAMERA_WIDTH + 10)) {
			goomba[i]->update(deltaTime);
			if (player->dies(glm::ivec2(goomba[i]->getPositionX(), goomba[i]->getPositionY()), goomba[i]->getSpriteSize()))
			{
				isDead = true;
				--lives;
				setPlayerIniPos();
			}
		}

	}

	if (player->getPositionX() >= 192 * (map->getTileSize()))
	{
		level++;
		loadLevel(level);
	}

	updateCamera(deltaTime);
	// si te quedas sin vidas retorna 0 que es volver al Menu
	return 1;	
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	projection = glm::ortho(cameraX, cameraX + float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	map->render();
	player->render();

	for (int i = 0; i < 1; i++)
	{
		if (goomba[i]->getPositionX() < cameraX + float(SCREEN_WIDTH + 10)) {
			goomba[i]->render();
		}
	}

	//textLifes.render("Lifes: " + to_string(lives), glm::vec2(10, 10 + 32), 32, glm::vec4(1, 1, 1, 1));
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

void Scene::updateCamera(int deltaTime)
{
	float playerX = float(player->getPositionX());
	if (playerX < CAMERA_WIDTH / 5 || frontalCamera)
		cameraX = 0;
	else if (playerX < map->getMapWidth() - 3 * CAMERA_WIDTH / 5)
	{
		if (playerX - cameraX < CAMERA_WIDTH / 5)
			cameraX = playerX - CAMERA_WIDTH / 5;
		if (playerX - cameraX > 2 * CAMERA_WIDTH / 5)
			cameraX = playerX - 2 * CAMERA_WIDTH / 5;
	}
}

void Scene::loadLevel(int lvl)
{
	if (map != NULL)
		delete map;
	map = TileMap::createTileMap("levels/level0" + to_string(lvl) + ".txt", glm::vec2(CAMERA_X, CAMERA_Y), texProgram);
	player->setTileMap(map);
	setPlayerIniPos();
	//if (level == 1) loadEnemies1();
	loadEnemies1();
	if (lvl == 2) frontalCamera = true;
	else frontalCamera = false;
}

void Scene::loadEnemies1()
{
	//Si lo pones en un fichero loadEnemies(int lvl) "/enemies" + to_string(lvl) + ".txt"
	/*
	ENEMIES
		20				-- number of goombas
		//pos goombas
		30				-- number of koopas
		// pos koopas
		1				-- number of lakitus
	*/

	for (int i = 0; i < 1/*NUMBER_OF_GOOMBAS*/; i++)
	{
		goomba[i] = new Goomba();
		goomba[i]->init(glm::ivec2(CAMERA_X, CAMERA_Y), texProgram);
		goomba[i]->setTileMap(map);
	}

	//FICAR LES COORDENADES INICALS DELS GOOMBAS EN ALGUN ARXIU EXTERN
	goomba[0]->setPosition(glm::vec2((6) * map->getTileSize(), 1 * map->getTileSize()));
	/*goomba[1]->setPosition(glm::vec2((8) * map->getTileSize(), 8 * map->getTileSize()));
	goomba[2]->setPosition(glm::vec2((10) * map->getTileSize(), 8 * map->getTileSize()));
	goomba[3]->setPosition(glm::vec2((9) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[4]->setPosition(glm::vec2((10) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[5]->setPosition(glm::vec2((11) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[6]->setPosition(glm::vec2((12) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[7]->setPosition(glm::vec2((13) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[8]->setPosition(glm::vec2((14) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[9]->setPosition(glm::vec2((15) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[10]->setPosition(glm::vec2((16) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[11]->setPosition(glm::vec2((17) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[12]->setPosition(glm::vec2((18) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[13]->setPosition(glm::vec2((19) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[14]->setPosition(glm::vec2((20) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[15]->setPosition(glm::vec2((21) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[16]->setPosition(glm::vec2((22) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[17]->setPosition(glm::vec2((23) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[18]->setPosition(glm::vec2((24) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	goomba[19]->setPosition(glm::vec2((66) * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));*/
}

void Scene::setPlayerIniPos()
{
	int iniX = map->getPosIniX();
	int iniY = map->getPosIniY();
	player->setPosition(glm::vec2(iniX * map->getTileSize(), iniY * map->getTileSize()));
}