#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "spreadGunBuff.h"




#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 0.12


enum BlockAnimations
{
	LIVING, ALIVE
};


void spreadGunBuff::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	hp = 10;
	start = 0;
	spriteSize = glm::ivec2(24, 15);
	spritesheet.loadFromFile("images/SpreadGunBuff.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(ALIVE, 8);
	sprite->addKeyframe(ALIVE, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(ALIVE, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(ALIVE, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(ALIVE, glm::vec2(0.75f, 0.0f));

	sprite->setAnimationSpeed(LIVING, 6);
	sprite->addKeyframe(LIVING, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(LIVING, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(LIVING, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(LIVING, glm::vec2(0.0f, 0.5f));

	sprite->changeAnimation(ALIVE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

// --
void spreadGunBuff::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (start == 1)
	{
		if (sprite->animation() != LIVING)
		{
			sprite->changeAnimation(LIVING);
		}
		else if (sprite->lastKeyFrame())
		{
			start = 2;
		}
	}
	else
	{
		if (sprite->animation() != ALIVE)
			sprite->changeAnimation(ALIVE);
	}
	if(start > 0)
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void spreadGunBuff::render()
{
	if ((start > 0) && hp > 0)
	{
		sprite->render();
	}
}

void spreadGunBuff::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void spreadGunBuff::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

int spreadGunBuff::getPositionX()
{
	return posPlayer.x;
}

int spreadGunBuff::getPositionY()
{
	return posPlayer.y;
}

glm::vec2 spreadGunBuff::getSpriteSize()
{
	return spriteSize;
}

bool spreadGunBuff::isDead()
{
	return (hp <= 0);
}

void spreadGunBuff::damage()
{
	if(start == 0)
		start = 1;
	hp--;
}