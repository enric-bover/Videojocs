#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 0.12


enum PlayerAnims
{
	STAND_RIGHT, MOVE_RIGHT, STAND_LEFT, MOVE_LEFT, JUMP_RIGHT, JUMP_LEFT, DEAD
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	dead = false;
	spritesheet.loadFromFile("images/tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteSize = glm::ivec2(20, 35);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.1, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);
	
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.0f));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 10);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 10);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.7f, 0.0f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.8f, 0.0f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.9f, 0.0f));

	sprite->setAnimationSpeed(JUMP_LEFT, 10);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.1f, 0.5f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3f, 0.5f));


	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.9f, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.7f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

// --
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	
	if (!bJumping) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= int(SPEED * deltaTime);
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(20, 35)))
			{
				posPlayer.x += int(SPEED * deltaTime);
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += int(SPEED * deltaTime);
			if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
			{
				posPlayer.x -= int(SPEED * deltaTime);
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMP_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
	}

	if (bJumping)
	{

		if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
		{
			sprite->changeAnimation(JUMP_LEFT);

		}
		else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
		{
			sprite->changeAnimation(JUMP_RIGHT);
		}
		else if (sprite->animation() == JUMP_RIGHT)
		{
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			posPlayer.x += int(SPEED * deltaTime);
		}
		if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
		{
			posPlayer.x -= int(SPEED * deltaTime);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			sprite->changeAnimation(JUMP_LEFT);
			posPlayer.x -= int(SPEED * deltaTime);
		}
		}
		else if (sprite->animation() == JUMP_LEFT)
		{
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			posPlayer.x -= int(SPEED * deltaTime);
		}
		if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
		{
			posPlayer.x += int(SPEED * deltaTime);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			sprite->changeAnimation(JUMP_RIGHT);
			posPlayer.x += int(SPEED * deltaTime);
		}
		}

		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(20, 35), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(20, 35), &posPlayer.y))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}


		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

int Player::getPositionX()
{
	return posPlayer.x;
}

int Player::getPositionY()
{
	return posPlayer.y;
}

bool Player::dies(const glm::vec2& posEnemie, const glm::ivec2& sizeTile)
{
	int xEnemie1, xEnemie2, yEnemie1, yEnemie2;
	xEnemie1 = posEnemie.x;
	xEnemie2 = xEnemie1 + sizeTile.x;
	yEnemie1 = posEnemie.y;
	yEnemie2 = yEnemie1 + sizeTile.y;
	//mirar primer si la posicio x1 del player es troba entre les del enemic, x2 player entre x del enemic
	//o com l'sprite del player es mes gran mirar si x1 < x1 enemie i x2 > x2 enemie
	if (((posPlayer.x >= xEnemie1) && (posPlayer.x <= xEnemie2)) || (((posPlayer.x + spriteSize.x) >= xEnemie1) && ((posPlayer.x + spriteSize.x) <= xEnemie2)) || ((posPlayer.x <= xEnemie1) && ((posPlayer.x + spriteSize.x) >= xEnemie2)))
	{
		if (((posPlayer.y >= yEnemie1) && (posPlayer.y <= yEnemie2)) || (((posPlayer.y + spriteSize.y) >= yEnemie1) && ((posPlayer.y + spriteSize.y) <= yEnemie2)) || ((posPlayer.y <= yEnemie1) && ((posPlayer.y + spriteSize.y) >= yEnemie2))) 
		{
			return true;
		}
	}
	return false;
	
}

