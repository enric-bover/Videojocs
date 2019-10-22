#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define MENU 0
#define EXIT -1
#define GAME 1

void Game::init()
{
	bPlay = true;
	glClearColor(93.f/255.f, 148.f/255.f, 251.f/255.f, 1.0f);
	scene.init();
	menu.init();
	screen = MENU;
}

bool Game::update(int deltaTime)
{
	if (screen == MENU) screen = menu.update(deltaTime);
	else if (screen == GAME) screen = scene.update(deltaTime);
	else bPlay = false;
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (screen == MENU) menu.render();
	else if (screen == GAME) scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





