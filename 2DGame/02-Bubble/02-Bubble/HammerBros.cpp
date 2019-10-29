#include "HammerBros.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Balas.h"

#define NUM_BALES 20
#define FALL_STEP 4
#define FRAME_SHOOT 20

enum HammerBrosAnims
{
	MOVE_RIGHT, MOVE_LEFT, ATTACK_RIGHT, ATTACK_LEFT, DIE_RIGHT, DIE_LEFT
};

void HammerBros::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	actionTimer = 0;
	hp = 3;
	dead = false;
	frontal = false;
	direction = MOVE_LEFT;
	spritesheet.loadFromFile("images/hammer_bros.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spriteSize = glm::ivec2(16, 24);

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
	
	for (int i = 0; i < NUM_BALES; i++)
	{
		bales[i] = new Balas();
		bales[i]->init(tileMapPos, shaderProgram);
		bales[i]->setTileMap(map);
	}
}

void HammerBros::update(int deltaTime, glm::ivec2 posEnemy)
{
	sprite->update(deltaTime);
	actionTimer += deltaTime;
	if (canShoot < FRAME_SHOOT)
		canShoot++;
	if (actualHammer >= NUM_BALES - 5) actualHammer = 0;
	if ((sprite->animation() == DIE_LEFT || sprite->animation() == DIE_RIGHT) && sprite->lastKeyFrame())
		dead = true;
	if (hp > 0) {
		this->posPlayer.y += FALL_STEP;
		bJumping = !map->collisionMoveDown(this->posPlayer, spriteSize, &(this->posPlayer.y));
		if (actionTimer > 3000) // decide new action
		{
			actionTimer = 0;
			float newAction = float(rand()) / float(RAND_MAX);
			if (newAction < 0.3f)
			{
				if (posPlayer.x > posEnemy.x)
				{
					if (posPlayer.x - posEnemy.x < 120 && posPlayer.x - posEnemy.x > 20)
						direction = ATTACK_LEFT;
					else
						direction = MOVE_LEFT;
				}
				else if (posPlayer.x < posEnemy.x)
				{
					if (posEnemy.x - posPlayer.x < 120 && posEnemy.x - posPlayer.x > 20)
						direction = ATTACK_RIGHT;
					else
						direction = MOVE_RIGHT;
				}
			}
			else if (newAction < 0.65)
				direction = MOVE_LEFT;
			else
				direction = MOVE_RIGHT;
		}
		// execute action
		if (map->collisionMoveLeft(this->posPlayer, spriteSize))
			direction = MOVE_RIGHT;
		else if (map->collisionMoveRight(this->posPlayer, spriteSize))
			direction = MOVE_LEFT;

		if (direction == MOVE_LEFT)
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			this->posPlayer.x -= 1;
		}
		else if (direction == MOVE_RIGHT)
		{
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			this->posPlayer.x += 1;
		}
		else if (direction == ATTACK_LEFT)
		{
			if (sprite->animation() != ATTACK_LEFT)
				sprite->changeAnimation(ATTACK_LEFT);
			shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), posEnemy);
		}
		else if (direction == ATTACK_RIGHT)
		{
			if (sprite->animation() != ATTACK_RIGHT)
				sprite->changeAnimation(ATTACK_RIGHT);
			shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), posEnemy);
		}
	}
	else
	{
		if ((direction == MOVE_LEFT || direction == ATTACK_LEFT) && sprite->animation() != DIE_LEFT)
			sprite->changeAnimation(DIE_LEFT);
		else if ((direction == MOVE_RIGHT || direction == ATTACK_RIGHT) && sprite->animation() != DIE_RIGHT)
			sprite->changeAnimation(DIE_RIGHT);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + this->posPlayer.x), float(tileMapDispl.y + this->posPlayer.y)));

	for (int i = 0; i < NUM_BALES; i++)
	{
		bales[i]->update(deltaTime);
	}
}

void HammerBros::render(float cameraX, float cameraWidht)
{
	if (posPlayer.x < cameraX + cameraWidht + 10)
	{
		if (hp > 0)
			sprite->render();
		else if (!dead)
			sprite->render();
		for (int i = 0; i < NUM_BALES; i++)
			bales[i]->render();
	}
}

void HammerBros::setMode(int mode) {
	if (mode == 1) frontal = false;
	else frontal = true;
}

bool HammerBros::collisionWithHammer(glm::ivec2 pos, glm::ivec2 size) {
	for (int i = 0; i < NUM_BALES; i++)
		if (bales[i]->kills(pos, size))
			return true;
	return false;
}

void HammerBros::shoot(const glm::vec2& pos, glm::ivec2 posEnemy)
{
	bool up = false;
	bool down = false;
	if (canShoot >= FRAME_SHOOT) {
		float x = posEnemy.x - posPlayer.x;
		float y = posEnemy.y - posPlayer.y;
		float mod = sqrt(x * x + y * y);
		glm::vec2 velocitat = glm::vec2(1.3f*x/mod, 1.3f*y/mod);
		if (frontal) 
			velocitat = glm::vec2(0, 1);
		bales[actualHammer]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 16), float(tileMapDispl.y + posPlayer.y + 10)), velocitat);
		canShoot = 0;
		actualHammer += 1;
	}
}