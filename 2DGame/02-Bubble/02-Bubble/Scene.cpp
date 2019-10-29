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

#define SIDE_SCROLLER 0
#define FRONTAL 1

Scene::Scene()
{
	map = NULL;
	player = NULL;
	first = true;
	level = 3;
	cameraState = SIDE_SCROLLER;
	cineTimer = 0.0f;
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
	engine = createIrrKlangDevice();
	engine->play2D("music/super_mario.mp3", true);
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

	if (level == 2)
	{
		sphere->update(deltaTime);
		if (!sphere->isDead())
		{
			if (player->kills(sphere->getPos(), sphere->getSpriteSize())) sphere->damage();
		}
	}

	if(level ==1)
	{
		interrogante->update(deltaTime);
		bool isDead = false;
		for (int i = 0; i < 1 && !isDead; i++) {
			if (goomba[i]->getPositionX() < cameraX + float(CAMERA_WIDTH + 10)) {
				goomba[i]->update(deltaTime);
				if (!goomba[i]->isDead())
				{
					if (player->dies(glm::ivec2(goomba[i]->getPositionX(), goomba[i]->getPositionY()), goomba[i]->getSpriteSize()))
					{
						isDead = true;
						setPlayerIniPos();
					}
					if (player->kills(glm::ivec2(goomba[i]->getPositionX(), goomba[i]->getPositionY()), goomba[i]->getSpriteSize()))
						goomba[i]->damage();
				}
			}
		}
		//koopas
		if (koopa->getPositionX() < cameraX + float(CAMERA_WIDTH + 10)) {
			koopa->update(deltaTime);
			if (!koopa->isDead())
			{
				if (player->dies(glm::ivec2(koopa->getPositionX(), koopa->getPositionY()), koopa->getSpriteSize()))
				{
					isDead = true;
					setPlayerIniPos();
				}
				if (player->kills(glm::ivec2(koopa->getPositionX(), koopa->getPositionY()), koopa->getSpriteSize()))
					koopa->damage();
			}
		}

		if (player->dies(glm::ivec2(interrogante->getPositionX(), interrogante->getPositionY()), interrogante->getSpriteSize()))
			interrogante->damage();
		if (player->dies(glm::ivec2(interrogante->getBuffPositionX(), interrogante->getBuffPositionY()), interrogante->getBuffSpriteSize()))
		{
			if (interrogante->isDead()) {
				interrogante->start();
				interrogante->update(deltaTime);
				if (interrogante->BuffisActive())
					player->activeTripleshoot();
			}
			if (interrogante->BuffisActive())
				interrogante->BuffDamage();
		}


		for (int i = 0; i < 5/*NUMBER_OF_HB*/ && !isDead; i++) {
			if (hammerBros[i]->getPositionX() < cameraX + float(CAMERA_WIDTH + 10))
			{
				hammerBros[i]->update(deltaTime, player->getPosition());
				if (!hammerBros[i]->isDead())
				{
					if (player->dies(hammerBros[i]->getPosition(), hammerBros[i]->getSpriteSize()) ||
						hammerBros[i]->collisionWithHammer(player->getPosition(), player->getSpriteSize()))
					{
						isDead = true;
						setPlayerIniPos();
					}
					if (player->kills(hammerBros[i]->getPosition(), hammerBros[i]->getSpriteSize()))
						hammerBros[i]->damage();
				}
			}
		}
	}
  
	if (player->getPositionY() >= 12.5 * (map->getTileSize())) //caes al vacio*
		setPlayerIniPos();


	if (level == 3)
	{	
		lakitu->update(deltaTime);
		if (!lakitu->isDead()) {
			if (player->dies(glm::ivec2(lakitu->getPositionX(), lakitu->getPositionY()), lakitu->getSpriteSize()))
			{

				setPlayerIniPos();
			}
			if (player->kills(glm::ivec2(lakitu->getPositionX(), lakitu->getPositionY()), lakitu->getSpriteSize()))
				lakitu->damage();
		}
		if (lakitu->activeEnemies())
		{
			int numEnemies = lakitu->getNumEnemies();
			for (int i = 0; i < numEnemies; i++)
			{
				if (lakitu->enemieLives(i))
				{
					glm::ivec2 posEnemie = glm::ivec2(lakitu->getPositionXEnemie(i), lakitu->getPositionYEnemie(i));
					if (player->dies(posEnemie, lakitu->getSpriteSizeEnemies()))
					{
						setPlayerIniPos();
					}
					if (player->kills(posEnemie, lakitu->getSpriteSizeEnemies()))
						lakitu->damageEnemie(i);
				}
			}
		}
	}

	
	if (level == 1 && goalState1()) 
		loadLevel(2);
	else if (level == 2 && goalState2()) {
		cineTimer += deltaTime;
		if (cineTimer < 50)
		{
			explosion->start();
			for (int i = 0; i < 5; ++i)
				hammerBros[i]->kill();
		}
		explosion->update(deltaTime);
		if (cineTimer > 300) {
			imageBgS2.loadFromFile("images/stage2_0.png", TEXTURE_PIXEL_FORMAT_RGBA);
			if (cineTimer > 1500) {
				player->setRunUpAnimation();
				if (player->getRound() == 1) imageBgS2.loadFromFile("images/stage2_1.png", TEXTURE_PIXEL_FORMAT_RGBA);
				else if (player->getRound() == 2) imageBgS2.loadFromFile("images/stage2_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
				else if (player->getRound() >= 3) loadLevel(3);
			}
		}
	}
	else if (level == 3 && goalState3()) return 0;
	
	updateCamera(deltaTime);
	return 1;	// si te quedas sin vidas retorna 0 que es volver al Menu
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

	if (level == 2) {
		bgS2->render(imageBgS2);
		for (int i = 0; i < 22 && !goalState2(); ++i) {
			fireBar->setPosition(glm::vec2(72 + 16 * (i%11), 155+(i/11)*16));
			fireBar->render(imageFireBar);
		}
	}
	if (level == 2) {
		sphere->render();
		explosion->setPosition(glm::ivec2(146, 96));
		explosion->render();
		explosion->setPosition(glm::ivec2(120, 81));
		explosion->render();
		explosion->setPosition(glm::ivec2(157, 107));
		explosion->render();
		explosion->setPosition(glm::ivec2(160, 70));
		explosion->render();
	}
	player->render();
	if (level == 1) {
		interrogante->render();

		for (int i = 0; i < 1/*NUMBER_OF_GOOMBAS*/; i++)
		{
			if (goomba[i]->getPositionX() < cameraX + float(SCREEN_WIDTH + 10)) {
				goomba[i]->render();
			}
		}
		for (int i = 0; i < 5; ++i)
			hammerBros[i]->render(cameraX, SCREEN_WIDTH);


		if (koopa->getPositionX() < cameraX + float(SCREEN_WIDTH + 10)) {
			koopa->render();
		}
		
	}

	if (level == 3)
		lakitu->render();
	


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
	if (playerX < CAMERA_WIDTH / 5 || level == 2) {
		cameraX = 0;
	}
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
	level = lvl;
	if (map != NULL)
		delete map;
	map = TileMap::createTileMap("levels/level0" + to_string(lvl) + ".txt", glm::vec2(CAMERA_X, CAMERA_Y), texProgram);
	player->setTileMap(map);
	player->setFrontal(false);
	setPlayerIniPos();
	if (lvl ==1)
		loadEnemies(level);

	if (lvl == 2)
	{
		cameraState = FRONTAL;
		player->setFrontal(true);

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(16 * 16, 16*11) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		imageBgS2.loadFromFile("images/stage2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		bgS2 = TexturedQuad::createTexturedQuad(geom, texCoords, &texProgram);
		bgS2->setPosition(glm::vec2(32, 32));

		glm::vec2 geom2[2] = { glm::vec2(0.f, 0.f), glm::vec2(16, 16) };
		glm::vec2 texCoords2[2] = { glm::vec2(1.f/8.f, 5.f/8.f), glm::vec2(2.f/8.f, 6.f/8.f) };
		imageFireBar.loadFromFile("images/tileset_lv1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		fireBar = TexturedQuad::createTexturedQuad(geom2, texCoords2, &texProgram);

		sphere = new Sphere();
		sphere->init(glm::ivec2(152, 104), texProgram);

		explosion = new Explosion();
		explosion->init(glm::ivec2(152, 104), texProgram);
		loadEnemies(level);
		
	}
	else if (lvl == 3)
	{
		loadEnemies(level);
		cameraState = FRONTAL;
		//player->setFrontal(true);


	}
	else cameraState = SIDE_SCROLLER;
}

void Scene::loadEnemies(int lvl)
{
	switch (lvl)
	{
	case 1:  
   
      
    interrogante = new block_interrogante();
	  interrogante->init(glm::ivec2(CAMERA_X, CAMERA_Y), texProgram);
	  interrogante->setTileMap(map);
	  interrogante->setPosition(glm::vec2((9) * map->getTileSize(), 9 * map->getTileSize()));
      
    koopa = new Koopa();
	  koopa->init(glm::ivec2(CAMERA_X, CAMERA_Y), texProgram);
	  koopa->setTileMap(map);
	  koopa->setPosition(glm::vec2((20) * map->getTileSize(), 1 * map->getTileSize()));
      
		for (int i = 0; i < 1/*NUMBER_OF_GOOMBAS*/; i++)
		{
			goomba[i] = new Goomba();
			goomba[i]->init(glm::ivec2(CAMERA_X, CAMERA_Y), texProgram);
			goomba[i]->setTileMap(map);
		}
		//FICAR LES COORDENADES INICALS DELS GOOMBAS EN ALGUN ARXIU EXTERN
		goomba[0]->setPosition(glm::vec2((6) * map->getTileSize(), 1 * map->getTileSize()));
		//goomba[1]->setPosition(glm::vec2((16) * map->getTileSize(), 8 * map->getTileSize()));
		//goomba[2]->setPosition(glm::vec2((24) * map->getTileSize(), 8 * map->getTileSize()));
		//goomba[3]->setPosition(glm::vec2((29) * map->getTileSize(), 7 * map->getTileSize()));
		//goomba[4]->setPosition(glm::vec2((35) * map->getTileSize(), 8 * map->getTileSize()));
		//goomba[5]->setPosition(glm::vec2((46) * map->getTileSize(), 5 * map->getTileSize()));
		//goomba[6]->setPosition(glm::vec2((51) * map->getTileSize(), 6 * map->getTileSize()));
		//goomba[7]->setPosition(glm::vec2((63) * map->getTileSize(), 7 * map->getTileSize()));
		//goomba[8]->setPosition(glm::vec2((67) * map->getTileSize(), 6 * map->getTileSize()));
		//goomba[9]->setPosition(glm::vec2((70) * map->getTileSize(), 7 * map->getTileSize()));
		//goomba[10]->setPosition(glm::vec2((86) * map->getTileSize(), 10 * map->getTileSize()));
		//goomba[11]->setPosition(glm::vec2((90) * map->getTileSize(), 7 * map->getTileSize()));
		//goomba[12]->setPosition(glm::vec2((102) * map->getTileSize(), 4 * map->getTileSize()));
		//goomba[13]->setPosition(glm::vec2((113) * map->getTileSize(), 6 * map->getTileSize()));
		//goomba[14]->setPosition(glm::vec2((120) * map->getTileSize(), 8 * map->getTileSize()));
		//goomba[15]->setPosition(glm::vec2((126) * map->getTileSize(), 5 * map->getTileSize()));
		//goomba[16]->setPosition(glm::vec2((137) * map->getTileSize(), 9 * map->getTileSize()));
		//goomba[17]->setPosition(glm::vec2((145) * map->getTileSize(), 5 * map->getTileSize()));
		//goomba[18]->setPosition(glm::vec2((156) * map->getTileSize(), 4 * map->getTileSize()));
		//goomba[19]->setPosition(glm::vec2((160) * map->getTileSize(), 7 * map->getTileSize()));

		for (int i = 0; i < 5/*NUMBER_OF_HB*/; i++)
		{
			hammerBros[i] = new HammerBros();
			hammerBros[i]->init(glm::ivec2(CAMERA_X, CAMERA_Y), texProgram);
			hammerBros[i]->setTileMap(map);
			hammerBros[i]->setMode(1);
		}
		hammerBros[0]->setPosition(glm::vec2((38) * map->getTileSize(), 10 * map->getTileSize()));
		hammerBros[1]->setPosition(glm::vec2((55) * map->getTileSize(), 10 * map->getTileSize()));
		hammerBros[2]->setPosition(glm::vec2((95) * map->getTileSize(), 10 * map->getTileSize()));
		hammerBros[3]->setPosition(glm::vec2((158) * map->getTileSize(), 10 * map->getTileSize()));
		hammerBros[4]->setPosition(glm::vec2((165) * map->getTileSize(), 10 * map->getTileSize()));
		break;
	case 2:
		for (int i = 0; i < 3/*NUMBER_OF_HB*/; i++)
		{
			hammerBros[i]->revive();
			hammerBros[i]->setTileMap(map);
			hammerBros[i]->setMode(0);
		}
		hammerBros[0]->setPosition(glm::vec2((7) * map->getTileSize(), 5 * map->getTileSize()));
		hammerBros[1]->setPosition(glm::vec2((8) * map->getTileSize(),  5* map->getTileSize()));
		hammerBros[2]->setPosition(glm::vec2((12) * map->getTileSize(), 5 * map->getTileSize()));

	case 3:
		lakitu = new Lakitu();
		lakitu->init(glm::ivec2(CAMERA_X, CAMERA_Y), texProgram);
		lakitu->setTileMap(map);
		lakitu->setPosition(glm::vec2((5) * map->getTileSize(), 1 * map->getTileSize()));
		lakitu->setMaxMinX(glm::vec2(1* map->getTileSize(), 17 * map->getTileSize()));
	default:
		break;
	}
}

void Scene::setPlayerIniPos()
{
	int iniX = map->getPosIniX();
	int iniY = map->getPosIniY();
	player->setPosition(glm::vec2(iniX * map->getTileSize(), iniY * map->getTileSize()));
}

bool Scene::goalState1()
{
	int tileSize = (map->getTileSize());
	int x = player->getPositionX();
	int y = player->getPositionY();
	return x >= 192 * tileSize && x <= (192 * tileSize + 16)  && y >= 10 * (tileSize);
}

bool Scene::goalState2() {
	return sphere->isDead();
}

bool Scene::goalState3() {
	return false;
}