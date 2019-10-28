#include "Koopa.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 0.12


enum KoopaAnimations
{
	MOVE_RIGHT, MOVE_LEFT, SHELL_RIGHT, SHELL_LEFT, DIE
};


void Koopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	hp = 8;
	dead = false;
	direction = MOVE_LEFT;
	spriteSize = glm::ivec2(16, 27);
	spritesheet.loadFromFile("images/koopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.1667, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1667, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1667, 0.0f));

	sprite->setAnimationSpeed(SHELL_RIGHT, 8);
	sprite->addKeyframe(SHELL_RIGHT, glm::vec2(0.3333f, 0.0f));
	sprite->addKeyframe(SHELL_RIGHT, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(SHELL_RIGHT, glm::vec2(0.6667f, 0.0f));
	sprite->addKeyframe(SHELL_RIGHT, glm::vec2(0.8333f, 0.0f));

	sprite->setAnimationSpeed(SHELL_LEFT, 8);
	sprite->addKeyframe(SHELL_LEFT, glm::vec2(0.3333f, 0.0f));
	sprite->addKeyframe(SHELL_LEFT, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(SHELL_LEFT, glm::vec2(0.6667f, 0.0f));
	sprite->addKeyframe(SHELL_LEFT, glm::vec2(0.8333f, 0.0f));



	sprite->setAnimationSpeed(DIE, 15);
	sprite->addKeyframe(DIE, glm::vec2(0.3333f, 0.5f));
	sprite->addKeyframe(DIE, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DIE, glm::vec2(0.6667f, 0.5f));
	sprite->addKeyframe(DIE, glm::vec2(0.8333f, 0.5f));


	sprite->changeAnimation(MOVE_LEFT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

// --
void Koopa::update(int deltaTime)
{

	sprite->update(deltaTime);
	if ((sprite->animation() == DIE) && sprite->lastKeyFrame())
		dead = true;
	if (hp > 4) {
		posPlayer.y += FALL_STEP;
		bJumping = !map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
		if (map->collisionMoveLeft(posPlayer, spriteSize))
		{
			direction = MOVE_RIGHT;
		}
		else if (map->collisionMoveRight(posPlayer, spriteSize))
		{
			direction = MOVE_LEFT;
		}

		if (direction == MOVE_LEFT)
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 1;
		}
		else
		{
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 1;
		}
	}
	else if (hp > 0)
	{
		if (map->collisionMoveLeft(posPlayer, spriteSize))
		{
			direction = MOVE_RIGHT;
		}
		else if (map->collisionMoveRight(posPlayer, spriteSize))
		{
			direction = MOVE_LEFT;
		}

		if (direction == MOVE_LEFT)
		{
			if (sprite->animation() != SHELL_LEFT)
				sprite->changeAnimation(SHELL_LEFT);
			posPlayer.x -= 2;
		}
		else
		{
			if (sprite->animation() != SHELL_RIGHT)
				sprite->changeAnimation(SHELL_RIGHT);
			posPlayer.x += 2;
		}
	}
	else
	{
		if (sprite->animation() != DIE)
			sprite->changeAnimation(DIE);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Koopa::render()
{
	if (hp > 0)
		sprite->render();
	else if (!dead)
		sprite->render();

}

void Koopa::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Koopa::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

int Koopa::getPositionX()
{
	return posPlayer.x;
}

int Koopa::getPositionY()
{
	return posPlayer.y;
}

glm::vec2 Koopa::getSpriteSize()
{
	return spriteSize;
}

bool Koopa::isDead()
{
	return (hp <= 0);
}

void Koopa::damage()
{
	hp--;
}