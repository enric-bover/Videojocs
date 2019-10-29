#include "Lakitu.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 0.12
#define FREQUENCY 120


enum LakituAnimations
{
	MOVE_RIGHT, MOVE_LEFT, THROW_RIGHT, THROW_LEFT, DIE_RIGHT, DIE_LEFT
};


void Lakitu::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram/*, const glm::ivec2& MinMaxX*/)
{
	bJumping = false;
	hp = 3;
	throwSpinny = false;
	countThrow = 0;
	movementy = 0;
	dead = false;
	direction = MOVE_LEFT;
	spriteSize = glm::ivec2(31, 45);
	spritesheet.loadFromFile("images/Lakitu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.1111f, 0.3333f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.8889f, 0.3333f));

	sprite->setAnimationSpeed(THROW_LEFT, 8);
	sprite->addKeyframe(THROW_LEFT, glm::vec2(0.1111f, 0.0f));
	sprite->addKeyframe(THROW_LEFT, glm::vec2(0.2222f, 0.0f));
	sprite->addKeyframe(THROW_LEFT, glm::vec2(0.3333f, 0.0f));
	sprite->addKeyframe(THROW_LEFT, glm::vec2(0.4444f, 0.0f));
	sprite->addKeyframe(THROW_LEFT, glm::vec2(0.5556f, 0.0f));
	sprite->addKeyframe(THROW_LEFT, glm::vec2(0.6667f, 0.0f));
	sprite->addKeyframe(THROW_LEFT, glm::vec2(0.7778f, 0.0f));
	sprite->addKeyframe(THROW_LEFT, glm::vec2(0.8889f, 0.0f));

	sprite->setAnimationSpeed(THROW_RIGHT, 8);
	sprite->addKeyframe(THROW_RIGHT, glm::vec2(0.7778f, 0.3333f));
	sprite->addKeyframe(THROW_RIGHT, glm::vec2(0.6667f, 0.3333f));
	sprite->addKeyframe(THROW_RIGHT, glm::vec2(0.5556f, 0.3333f));
	sprite->addKeyframe(THROW_RIGHT, glm::vec2(0.4444f, 0.3333f));
	sprite->addKeyframe(THROW_RIGHT, glm::vec2(0.3333f, 0.3333f));
	sprite->addKeyframe(THROW_RIGHT, glm::vec2(0.2222f, 0.3333f));
	sprite->addKeyframe(THROW_RIGHT, glm::vec2(0.1111f, 0.3333f));
	sprite->addKeyframe(THROW_RIGHT, glm::vec2(0.0f, 0.3333f));


	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.0f, 0.6666f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.1111f, 0.6666f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.2222f, 0.6666f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.3333f, 0.6666f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.4444f, 0.6666f));

	sprite->setAnimationSpeed(DIE_LEFT, 8);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.8889f, 0.6666f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.7778f, 0.6666f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.6667f, 0.6666f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.5556f, 0.6666f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.4444f, 0.6666f));


	sprite->changeAnimation(MOVE_LEFT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	for (int i = 0; i < 5; i++)
	{
		spinny[i] = new Spinny();
		spinny[i]->init(tileMapPos, shaderProgram);
	}


}

// --
void Lakitu::update(int deltaTime)
{
	movementy++;
	countThrow++;
	if (countThrow >= FREQUENCY)
	{
		countThrow = 0;
		throwSpinny = true;
	}

	if (movementy >= 24) movementy = 0;
	if (movementy <= 11)
	{
		posPlayer.y += (movementy % 3)%2;
	}
	else
	{
		posPlayer.y -= (movementy % 3) % 2;
	}
	int frame = sprite->getCurrentKeyFrame();
	if (hp > 0)
	{
		if (direction == MOVE_LEFT)
		{
			if (throwSpinny)
			{

				if (sprite->animation() == THROW_RIGHT)
				{
					sprite->changeAnimation(THROW_LEFT);
					sprite->setCurrentFrame(frame);
				}
				else if (sprite->animation() != THROW_LEFT)
					sprite->changeAnimation(THROW_LEFT);

				if (sprite->lastKeyFrame())
					throwSpinny = false;
			}
			else if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 1;
		}
		else
		{
			if (throwSpinny)
			{
				if (sprite->animation() == THROW_LEFT)
				{
					sprite->changeAnimation(THROW_RIGHT);
					sprite->setCurrentFrame(frame);
				}
				else if (sprite->animation() != THROW_RIGHT)
					sprite->changeAnimation(THROW_RIGHT);
				else if (sprite->lastKeyFrame())
					throwSpinny = false;
			}
			else if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 1;
		}
	}
	else
	{
		if (direction == MOVE_LEFT)
			if (sprite->animation() != DIE_LEFT)
				sprite->changeAnimation(DIE_LEFT);
			else if (sprite->lastKeyFrame())
				dead = true;
		if (direction == MOVE_RIGHT)
			if (sprite->animation() != DIE_RIGHT)
				sprite->changeAnimation(DIE_RIGHT);
			else if (sprite->lastKeyFrame())
				dead = true;
	}
	sprite->update(deltaTime);
	
	if (posPlayer.x < MinX)
		direction = MOVE_RIGHT;
	else if (posPlayer.x > MaxX)
		direction = MOVE_LEFT;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Lakitu::render()
{
	if (hp > 0)
		sprite->render();
	else if (!dead)
		sprite->render();

}

void Lakitu::setTileMap(TileMap* tileMap)
{
	map = tileMap;
	for (int i = 0; i < 5; i++)
	{
		spinny[i]->setTileMap(tileMap);
	}
}

void Lakitu::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	for (int i = 0; i < 5; i++)
	{
		spinny[i]->setPosition(pos);
	}
}

void Lakitu::setMaxMinX(const glm::vec2& pos)
{
	MinX = pos.x;
	MaxX = pos.y;
}

int Lakitu::getPositionX()
{
	return posPlayer.x;
}

int Lakitu::getPositionY()
{
	return posPlayer.y;
}

glm::vec2 Lakitu::getSpriteSize()
{
	return spriteSize;
}

bool Lakitu::isDead()
{
	return (hp <= 0);
}

void Lakitu::damage()
{
	hp--;
}

void Lakitu::activateSpinny(int number)
{
	spinny[number]->activate();
}
