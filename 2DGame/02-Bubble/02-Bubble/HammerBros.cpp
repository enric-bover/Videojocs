#include "HammerBros.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Balas.h"

#define NUM_BALES 90
#define FALL_STEP 4


enum HammerBrosAnims
{
	MOVE_RIGHT, MOVE_LEFT, ATTACK_RIGHT, ATTACK_LEFT, DIE_RIGHT, DIE_LEFT
};

void HammerBros::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	hp = 5;
	dead = false;
	direction = MOVE_LEFT;
	spritesheet.loadFromFile("images/hammer_bros.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spriteSize = glm::ivec2(16, 24);

	//for (int i = 0; i < NUM_BALES; i++)
	//{
		//bales[i] = new Balas();
		//bales[i]->init(tileMapPos, shaderProgram);
		//bales[i]->setTileMap(map);
	//}

	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 8);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 8);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.0f, 0.25f));

	sprite->setAnimationSpeed(DIE_LEFT, 8);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0.75f, 0.5f));

	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.75f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.25f, 0.75f));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.0f, 0.75f));

	sprite->changeAnimation(direction);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void HammerBros::update(int deltaTime)
{
	sprite->update(deltaTime);
	if ((sprite->animation() == DIE_LEFT || sprite->animation() == DIE_RIGHT) && sprite->lastKeyFrame())
		dead = true;
	if (hp > 0) {
		posPlayer.y += FALL_STEP;
		bJumping = !map->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
		if (map->collisionMoveLeft(posPlayer, spriteSize))
			direction = MOVE_RIGHT;
		else if (map->collisionMoveRight(posPlayer, spriteSize))
			direction = MOVE_LEFT;

		if (direction == MOVE_LEFT)
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 1;
		}
		else if (direction == MOVE_RIGHT)
		{
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 1;
		}
		else if (direction == ATTACK_LEFT && sprite->lastKeyFrame())
		{
			sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 1;

		}
		else
		{
			sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x -= 1;
		}

		//if (sprite->lastKeyFrame())
		//{
			//if (direction == MOVE_LEFT && ((double)rand() / (RAND_MAX)) < 0.005)
				//direction = ATTACK_LEFT;
			//else if (direction == MOVE_RIGHT && ((double)rand() / (RAND_MAX)) < 0.005)
				//direction = ATTACK_RIGHT;
		//}
	}
	else
	{
		if (direction == DIE_LEFT && sprite->animation() != DIE_LEFT)
			sprite->changeAnimation(DIE_LEFT);
		else if (direction == DIE_RIGHT && sprite->animation() != DIE_RIGHT)
			sprite->changeAnimation(DIE_RIGHT);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

//void HammerBros::render()
//{
	//if (!dead)
		//sprite->render();
//}

