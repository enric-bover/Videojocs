#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "block_interrogante.h"
#include "spreadGunBuff.h"



#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 0.12


enum BlockAnimations
{
	ALIVE, DEAD
};


void block_interrogante::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	hp = 1;
	buff = new spreadGunBuff;
	buff->init(tileMapPos, shaderProgram);
	spriteSize = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/bloque_interrogante.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.2f, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(ALIVE, 8);
	sprite->addKeyframe(ALIVE, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(ALIVE, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(ALIVE, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(ALIVE, glm::vec2(0.6f, 0.0f));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.8f, 0.0f));

	sprite->changeAnimation(ALIVE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

// --
void block_interrogante::update(int deltaTime)
{

	sprite->update(deltaTime);
	buff->update(deltaTime);
	if (hp <= 0)
	{
			sprite->changeAnimation(DEAD);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void block_interrogante::render()
{
		sprite->render();
		buff->render();
}

void block_interrogante::setTileMap(TileMap* tileMap)
{
	buff->setTileMap(tileMap);
	map = tileMap;
}

void block_interrogante::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	buff->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 4), float(tileMapDispl.y + posPlayer.y - 16)));
}

int block_interrogante::getPositionX()
{
	return posPlayer.x;
}

int block_interrogante::getPositionY()
{
	return posPlayer.y;
}

glm::vec2 block_interrogante::getSpriteSize()
{
	return spriteSize;
}

int block_interrogante::getBuffPositionX()
{
	return buff->getPositionX();
}

int block_interrogante::getBuffPositionY()
{
	return buff->getPositionY();
}

glm::vec2 block_interrogante::getBuffSpriteSize()
{
	return buff->getSpriteSize();
}

bool block_interrogante::isDead()
{
	return (hp <= 0);
}

bool block_interrogante::BuffisActive()
{
	return buff->isActive();
}

void block_interrogante::start()
{
	buff->activate();
}

void block_interrogante::damage()
{
	//buff->damage();
	hp--;
}

void block_interrogante::BuffDamage()
{
	buff->damage();
}