#include "Spinny.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 0.12


enum SpinnyAnimations
{
	MOVE_RIGHT, MOVE_LEFT, FALL
};


void Spinny::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	hp = 0;
	dead = true;
	first_contact = false;
	direction = MOVE_LEFT;
	spriteSize = glm::ivec2(20, 18);
	spritesheet.loadFromFile("images/Spinny.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.1429, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1429f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1429f, 0.0f));

	sprite->setAnimationSpeed(FALL, 15);
	sprite->addKeyframe(FALL, glm::vec2(0.2857f, 0.0f));
	sprite->addKeyframe(FALL, glm::vec2(0.4286f, 0.0f));
	sprite->addKeyframe(FALL, glm::vec2(0.5714, 0.0f));
	sprite->addKeyframe(FALL, glm::vec2(0.7143, 0.0f));





	sprite->changeAnimation(FALL);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

// --
void Spinny::update(int deltaTime)
{
	if (!dead)
	{
		sprite->update(deltaTime);
		if (hp > 0) {
			posPlayer.y += FALL_STEP -2;
			bJumping = !map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
			if (!first_contact)
			{
				if (sprite->animation() != FALL)
					sprite->changeAnimation(FALL);
			}
			if (!bJumping)
				first_contact = true;
			if (first_contact && bJumping)
			{
				posPlayer.y -= FALL_STEP -2;
				if (direction == MOVE_RIGHT)
					direction = MOVE_LEFT;
				else direction = MOVE_RIGHT;
			}

			if (map->collisionMoveLeft(posPlayer, spriteSize))
			{
				direction = MOVE_RIGHT;
			}
			else if (map->collisionMoveRight(posPlayer, spriteSize))
			{
				direction = MOVE_LEFT;
			}
			if (first_contact)
			{
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
			
		}
		if (posPlayer.y > (13.5 * map->getTileSize()))
		{
			dead = true;
			hp = 0;
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}

}

void Spinny::render()
{
	 if (!dead)
		sprite->render();

}

void Spinny::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Spinny::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	dead = false;
	first_contact = false;
	hp = 2;
	bJumping = false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

int Spinny::getPositionX()
{
	return posPlayer.x;
}

int Spinny::getPositionY()
{
	return posPlayer.y;
}

glm::vec2 Spinny::getSpriteSize()
{
	return spriteSize;
}

bool Spinny::isDead()
{
	return (hp <= 0);
}

void Spinny::damage()
{
	hp--;
	if (hp <= 0) dead = true;
}


bool Spinny::isActive()
{
	return !dead;
}
