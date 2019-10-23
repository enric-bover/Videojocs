#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"

// --
#define CAMERA_X 0
#define CAMERA_Y 0
#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240
#define POINT_WIDTH 16
#define POINT_HEIGHT 10

#define OP_PLAY 0
#define OP_INSTRUCTIONS 1
#define OP_CREDITS 2
#define OP_EXIT 3

#define SCREEN_MENU 0
#define SCREEN_INSTRUCTIONS 1
#define SCREEN_CREDITS 2

#define LIMIT_TIMER 100

Menu::Menu()
{
	//quad = NULL;
}

Menu::~Menu()
{
	//if (quad != NULL)
	//	delete quad;
}


void Menu::init()
{
	initShaders();

	selectedOption = OP_PLAY;
	currentScreen = SCREEN_MENU;

	// load imageS
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(CAMERA_WIDTH, CAMERA_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	image[SCREEN_MENU].loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	image[SCREEN_INSTRUCTIONS].loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	image[SCREEN_CREDITS].loadFromFile("images/creditos.png", TEXTURE_PIXEL_FORMAT_RGBA);


	glm::vec2 geom2[2] = { glm::vec2(0.f, 0.f), glm::vec2(16.0f, 10.0f) };
	glm::vec2 texCoords2[2] = { glm::vec2(0.0f, 5.0f / 8.f), glm::vec2(1.0f / 8.0f, 6.0f / 8.0f) };
	cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
	imageCursor.loadFromFile("images/tileset_lv1.png", TEXTURE_PIXEL_FORMAT_RGBA);

	updatePosPoint();

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	timer = 0.0f;
	startTimer = false;
}

// 0 continua, 1 play, -1 exit
int Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	if (startTimer && timer < LIMIT_TIMER) {
		timer += deltaTime;
		return 0;
	}
	startTimer = false;
	timer = 0.0f;
	if (currentScreen == SCREEN_MENU) {
		startTimer = true;
		if (Game::instance().getSpecialKey(0x67)) {
			selectedOption = (++selectedOption) % 4;
		}
		else if (Game::instance().getSpecialKey(0x65))
		{
			selectedOption = (--selectedOption);
			if (selectedOption < OP_PLAY) selectedOption = OP_EXIT;
		}
		else if (Game::instance().getKey(13)) {
			if (selectedOption == OP_EXIT) return -1;
			else if (selectedOption == OP_PLAY) return 1;
			currentScreen = selectedOption;
		}

	}
	else if (currentScreen == SCREEN_INSTRUCTIONS || currentScreen == SCREEN_CREDITS) {
		startTimer = true;
		if (Game::instance().getKey(13)) {
			currentScreen = SCREEN_MENU;
		}
	}

	updatePosPoint();

	return 0;
}

void Menu::render()
{
	glm::mat4 modelview;

	texProgram.use();
	projection = glm::ortho(0.f, float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	// render background
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	background->render(image[currentScreen]);
	//render point
	modelview = glm::translate(glm::mat4(1.f), glm::vec3(posPoint, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	cursor->render(imageCursor);
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Menu::updatePosPoint()
{
	if (currentScreen == SCREEN_INSTRUCTIONS || currentScreen == SCREEN_CREDITS)
		posPoint = glm::vec2(235.0f, 217.0f);
	else
		posPoint = glm::vec2(55.0f, 120.0f + 19.0f * selectedOption);
}