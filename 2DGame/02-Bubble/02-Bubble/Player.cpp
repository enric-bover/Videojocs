#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Balas.h"



#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 0.12
#define FRAME_SHOOT 20

//DI: diagonal
//U: up
//D: down
//R: right
//L: left
//S: straight
enum PlayerAnims
{
	STAND_RIGHT, MOVE_RIGHT, STAND_LEFT, MOVE_LEFT, JUMP_RIGHT, JUMP_LEFT, SHOOT_DI_U_L, SHOOT_DI_U_R, STAND_SHOOT_DI_U_L, STAND_SHOOT_DI_U_R
};

enum PlayerAnims2
{
	SHOOT_S_L, SHOOT_S_R, SHOOT_DI_D_L, SHOOT_DI_D_R, SHOOT_STAND_S_L, SHOOT_STAND_S_R, STAND_SHOOT_DI_D_L, STAND_SHOOT_DI_D_R
};

enum PlayerAnims3
{
	F_STAND, F_DOWN, F_RUN_UP, F_MOVE_L, F_MOVE_R
};

enum Shooting
{
	UP_R, UP_L, R, L, DOWN_R, DOWN_L, UP
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	dead = false; 
	right = true;
	chooseSprite = 1;
	actualBullet = 0;
	canShoot = 0;
	tripleshoot = false;
	frontal = false;
	runUp = false;
	round = 0;
	spritesheet.loadFromFile("images/tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet2.loadFromFile("images/ShootingStraight+DiagonalDown.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet3.loadFromFile("images/sprites_player_lvl2.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spriteSize = glm::ivec2(20, 35);
	spriteSize2 = glm::ivec2(25, 35);
	spriteSize3 = glm::ivec2(20, 44);

	for (int i = 0; i < NUM_BALES; i++)
	{
		bales[i] = new Balas();
		bales[i]->init(tileMapPos, shaderProgram);
		bales[i]->setTileMap(map);
	}

	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.1, 0.33), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);
	
	sprite2 = Sprite::createSprite(spriteSize2, glm::vec2(0.1667, 0.5), &spritesheet2, &shaderProgram);
	sprite2->setNumberAnimations(8);

	sprite3 = Sprite::createSprite(spriteSize3, glm::vec2(0.1667, 0.5), &spritesheet3, &shaderProgram);
	sprite3->setNumberAnimations(5);

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
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
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.0f, 0.3333f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.1f, 0.3333f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.2f, 0.3333f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.3f, 0.3333f));


	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.9f, 0.3333f));

	sprite->setAnimationSpeed(MOVE_LEFT, 10);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8f, 0.3333f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.7f, 0.3333f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.3333f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.3333f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.3333f));


	sprite->setAnimationSpeed(STAND_SHOOT_DI_U_L, 10);
	sprite->addKeyframe(STAND_SHOOT_DI_U_L, glm::vec2(0.5f, 0.6666f));


	sprite->setAnimationSpeed(STAND_SHOOT_DI_U_R, 10);
	sprite->addKeyframe(STAND_SHOOT_DI_U_R, glm::vec2(0.0f, 0.6666f));

	sprite->setAnimationSpeed(SHOOT_DI_U_R, 10);
	sprite->addKeyframe(SHOOT_DI_U_R, glm::vec2(0.0f, 0.6666f));
	sprite->addKeyframe(SHOOT_DI_U_R, glm::vec2(0.1f, 0.6666f));
	sprite->addKeyframe(SHOOT_DI_U_R, glm::vec2(0.2f, 0.6666f));

	sprite->setAnimationSpeed(SHOOT_DI_U_L, 10);
	sprite->addKeyframe(SHOOT_DI_U_L, glm::vec2(0.3f, 0.6666f));
	sprite->addKeyframe(SHOOT_DI_U_L, glm::vec2(0.4f, 0.6666f));
	sprite->addKeyframe(SHOOT_DI_U_L, glm::vec2(0.5f, 0.6666f));

	//-------------------------------------------------------- SECOND SPRITE CHARGE

	sprite2->setAnimationSpeed(SHOOT_S_R, 10);
	sprite2->addKeyframe(SHOOT_S_R, glm::vec2(0.0f, 0.0f));
	sprite2->addKeyframe(SHOOT_S_R, glm::vec2(0.1667f, 0.0f));
	sprite2->addKeyframe(SHOOT_S_R, glm::vec2(0.3333f, 0.0f));


	sprite2->setAnimationSpeed(SHOOT_STAND_S_R, 10);
	sprite2->addKeyframe(SHOOT_STAND_S_R, glm::vec2(0.0f, 0.0f));

	sprite2->setAnimationSpeed(SHOOT_S_L, 10);
	sprite2->addKeyframe(SHOOT_S_L, glm::vec2(0.8333f, 0.0f));
	sprite2->addKeyframe(SHOOT_S_L, glm::vec2(0.6667f, 0.0f));
	sprite2->addKeyframe(SHOOT_S_L, glm::vec2(0.5f, 0.0f));

	sprite2->setAnimationSpeed(SHOOT_STAND_S_L, 10);
	sprite2->addKeyframe(SHOOT_STAND_S_L, glm::vec2(0.8333f, 0.0f));

	sprite2->setAnimationSpeed(SHOOT_DI_D_R, 10);
	sprite2->addKeyframe(SHOOT_DI_D_R, glm::vec2(0.0f, 0.5f));
	sprite2->addKeyframe(SHOOT_DI_D_R, glm::vec2(0.1667f, 0.5f));
	sprite2->addKeyframe(SHOOT_DI_D_R, glm::vec2(0.3333f, 0.5f));

	sprite2->setAnimationSpeed(STAND_SHOOT_DI_D_R, 10);
	sprite2->addKeyframe(STAND_SHOOT_DI_D_R, glm::vec2(0.0f, 0.5f));


	sprite2->setAnimationSpeed(STAND_SHOOT_DI_D_L, 10);
	sprite2->addKeyframe(STAND_SHOOT_DI_D_L, glm::vec2(0.8333f, 0.5f));

	sprite2->setAnimationSpeed(SHOOT_DI_D_L, 10);
	sprite2->addKeyframe(SHOOT_DI_D_L, glm::vec2(0.5f, 0.5f));
	sprite2->addKeyframe(SHOOT_DI_D_L, glm::vec2(0.6667f, 0.5f));
	sprite2->addKeyframe(SHOOT_DI_D_L, glm::vec2(0.8333f, 0.5f));

	//-------------------------------------------------------- THIRD SPRITE CHARGE
	sprite3->setAnimationSpeed(F_STAND, 8);
	sprite3->addKeyframe(F_STAND, glm::vec2(0.0f, 0.0f));

	sprite3->setAnimationSpeed(F_DOWN, 8);
	sprite3->addKeyframe(F_DOWN, glm::vec2(0.1667f, 0.0f));

	sprite3->setAnimationSpeed(F_RUN_UP, 10);
	sprite3->addKeyframe(F_RUN_UP, glm::vec2(0.3333f, 0.0f));
	sprite3->addKeyframe(F_RUN_UP, glm::vec2(0.5f, 0.0f));
	
	sprite3->setAnimationSpeed(F_MOVE_L, 8);
	sprite3->addKeyframe(F_MOVE_L, glm::vec2(0.3333f, 0.5f));
	sprite3->addKeyframe(F_MOVE_L, glm::vec2(0.1667f, 0.5f));
	sprite3->addKeyframe(F_MOVE_L, glm::vec2(0.0000f, 0.5f));

	sprite3->setAnimationSpeed(F_MOVE_R, 8);
	sprite3->addKeyframe(F_MOVE_R, glm::vec2(0.5f, 0.5f));
	sprite3->addKeyframe(F_MOVE_R, glm::vec2(0.6667f, 0.5f));
	sprite3->addKeyframe(F_MOVE_R, glm::vec2(0.8333f, 0.5f));

		
	sprite->changeAnimation(0);
	sprite2->changeAnimation(0);
	sprite3->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite2->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite3->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));


}

// --
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite2->update(deltaTime);
	sprite3->update(deltaTime);
	if (canShoot < FRAME_SHOOT)
		canShoot++;
	if (actualBullet >= NUM_BALES - 5) actualBullet = 0;
	if (frontal) {
		chooseSprite = 3;
		if (Game::instance().getKey(' '))
			shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), UP);
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (sprite3->animation() != F_MOVE_L)
				sprite3->changeAnimation(F_MOVE_L);
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
			{
				posPlayer.x += 2;
				sprite3->changeAnimation(F_STAND);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (sprite3->animation() != F_MOVE_R)
				sprite3->changeAnimation(F_MOVE_R);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			{
				posPlayer.x -= 2;
				sprite3->changeAnimation(F_STAND);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			sprite3->changeAnimation(F_DOWN);
		else if (runUp) {
			if (sprite3->animation() != F_RUN_UP) {
				sprite3->changeAnimation(F_RUN_UP);
				startY = posPlayer.y;
				round = 0;
			}
			posPlayer.y -= 1;
			if (posPlayer.y < startY - 40) {
				posPlayer.y = startY;
				++round;
			}
		
		}
		else
			sprite3->changeAnimation(F_STAND);
	}
	else if (!bJumping) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			posPlayer.x -= int(SPEED * deltaTime);
			right = false;
			if (Game::instance().getKey(' '))		//dispares
			{
				if (Game::instance().getKey('w'))
				{
					shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), UP_L);
					if (sprite->animation() != SHOOT_DI_U_L)
						sprite->changeAnimation(SHOOT_DI_U_L);
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(20, 35)))
					{
						sprite->changeAnimation(STAND_SHOOT_DI_U_L);
						posPlayer.x += int(SPEED * deltaTime);
					}
					chooseSprite = 1;
				}
				else if (Game::instance().getKey('s'))
				{
					shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), DOWN_L);
					if (sprite2->animation() != SHOOT_DI_D_L)
						sprite2->changeAnimation(SHOOT_DI_D_L);
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(20, 35)))
					{
						sprite2->changeAnimation(STAND_SHOOT_DI_D_L);
						posPlayer.x += int(SPEED * deltaTime);
					}
					chooseSprite = 2;
				}
				else
				{
					shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), L);
					if (sprite2->animation() != SHOOT_S_L)
						sprite2->changeAnimation(SHOOT_S_L);
					if (map->collisionMoveLeft(posPlayer, glm::ivec2(20, 35)))
					{
						sprite2->changeAnimation(SHOOT_STAND_S_L);
						posPlayer.x += int(SPEED * deltaTime);
					}
					chooseSprite = 2;
				}
			}
			else
			{
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(20, 35)))
				{
					posPlayer.x += int(SPEED * deltaTime);
					sprite->changeAnimation(STAND_LEFT);
				}
				chooseSprite = 1;

			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			posPlayer.x += int(SPEED * deltaTime);
			right = true;
			if (Game::instance().getKey(' '))		//dispares
			{
				if (Game::instance().getKey('w'))
				{
					shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), UP_R);
					if (sprite->animation() != SHOOT_DI_U_R)
						sprite->changeAnimation(SHOOT_DI_U_R);
					if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
					{
						sprite->changeAnimation(STAND_SHOOT_DI_U_R);
						posPlayer.x -= int(SPEED * deltaTime);
					}
					chooseSprite = 1;
				}
				else if (Game::instance().getKey('s'))
				{
					shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), DOWN_R);
					if (sprite2->animation() != SHOOT_DI_D_R)
						sprite2->changeAnimation(SHOOT_DI_D_R);
					if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
					{
						sprite2->changeAnimation(STAND_SHOOT_DI_D_R);
						posPlayer.x -= int(SPEED * deltaTime);
					}
					chooseSprite = 2;
				}
				else
				{
					shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), R);
					if (sprite2->animation() != SHOOT_S_R)
						sprite2->changeAnimation(SHOOT_S_R);
					if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
					{
						sprite2->changeAnimation(SHOOT_STAND_S_R);
						posPlayer.x -= int(SPEED * deltaTime);
					}
					chooseSprite = 2;
				}
			}
			else
			{
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
				if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
				{
					posPlayer.x -= int(SPEED * deltaTime);
					sprite->changeAnimation(STAND_RIGHT);
				}
				chooseSprite = 1;
			}
		}
		else
		{
			if (!Game::instance().getKey(' '))
			{
				if (right)
				{
					sprite->changeAnimation(STAND_RIGHT);
					right = true;
					chooseSprite = 1;

				}
				else
				{
					sprite->changeAnimation(STAND_LEFT);
					right = false;
					chooseSprite = 1;


				}
			}
			else
			{
				if (right)
				{
					if (Game::instance().getKey('w'))
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), UP_R);
						sprite->changeAnimation(STAND_SHOOT_DI_U_R);
						chooseSprite = 1;
					}
					else if (Game::instance().getKey('s'))
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), DOWN_R);
						sprite2->changeAnimation(STAND_SHOOT_DI_D_R);
						chooseSprite = 2;
					}
					else 
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), R);
						sprite2->changeAnimation(SHOOT_STAND_S_R);
						chooseSprite = 2;
					}
					right = true;
				}
				else
				{
					if (Game::instance().getKey('w'))
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), UP_L);
						sprite->changeAnimation(STAND_SHOOT_DI_U_L);
						chooseSprite = 1;
					}
					else if (Game::instance().getKey('s'))
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), DOWN_L);
						sprite2->changeAnimation(STAND_SHOOT_DI_D_L);
						chooseSprite = 2;
					}
					else
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), L);
						sprite2->changeAnimation(SHOOT_STAND_S_L);
						chooseSprite = 2;
					}
					right = false;
				}
			}

		}
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
	else
	{
			if (sprite->animation() == JUMP_RIGHT)
			{
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
				{
					right = true;
					posPlayer.x += int(SPEED * deltaTime);
				}
				else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
				{
					right = false;
					sprite->changeAnimation(JUMP_LEFT);
					posPlayer.x -= int(SPEED * deltaTime);
				}
				if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
				{
					posPlayer.x -= int(SPEED * deltaTime);
				}
			}
			else if (sprite->animation() == JUMP_LEFT)
			{
				if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
				{
					right = false;
					posPlayer.x -= int(SPEED * deltaTime);
				}
				else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
				{
					right = true;
					sprite->changeAnimation(JUMP_RIGHT);
					posPlayer.x += int(SPEED * deltaTime);
				}
				if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
				{
					posPlayer.x += int(SPEED * deltaTime);
				}
			}
			else if (right)
			{
				right = true;
				sprite->changeAnimation(JUMP_RIGHT);
			}
			else
			{
				right = false;
				sprite->changeAnimation(JUMP_LEFT);
			}

			if (right)
			{
				if (Game::instance().getKey(' '))
				{
					if (Game::instance().getKey('w'))
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), UP_R);
					}
					else if (Game::instance().getKey('s'))
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), DOWN_R);
					}
					else
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), R);
					}
				}
			}
			else
			{
				if (Game::instance().getKey(' '))
				{
					if (Game::instance().getKey('w'))
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), UP_L);
					}
					else if (Game::instance().getKey('s'))
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), DOWN_L);
					}
					else
					{
						shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)), L);
					}
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
		chooseSprite = 1;
	}
	
	if (Game::instance().getKey('t'))
	{
		tripleshoot = true;
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite2->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite3->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	for (int i = 0; i < NUM_BALES; i++)
	{
		bales[i]->update(deltaTime);
	}
}

void Player::render()
{
	if (chooseSprite == 1)
		sprite->render();
	else if (chooseSprite == 2)
		sprite2->render();
	else
		sprite3->render();
	for (int i = 0; i < NUM_BALES; i++)
	{
		bales[i]->render();

	}
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite2->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
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
	//hago el tama�o de colision mas peque�o para que no se note tanto que tenemos pixeles vacios delante del sprite del player
	int xEnemie1, xEnemie2, yEnemie1, yEnemie2;
	xEnemie1 = posEnemie.x + 4;
	xEnemie2 = xEnemie1 + sizeTile.x - 2;
	yEnemie1 = posEnemie.y + 2;
	yEnemie2 = yEnemie1 + sizeTile.y - 2;
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

bool Player::kills(const glm::vec2& posEnemie, const glm::ivec2& sizeTile) 
{
	for (int i = 0; i < NUM_BALES; i++)
	{
		if (bales[i]->kills(posEnemie, sizeTile))
		{
			return true;
		}
	}
	return false;
}

void Player::shoot(const glm::vec2& pos, int angle) 
{
	bool up = false;
	bool down = false;
	if (canShoot >= FRAME_SHOOT){
		glm::ivec2 velocitat = glm::vec2(0, 0);
		if (angle == R)
		{
			velocitat.x = 2;
			bales[actualBullet]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 16), float(tileMapDispl.y + posPlayer.y + 10)), velocitat);
		}
		else if (angle == L)
		{
			velocitat.x = -2;
			bales[actualBullet]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x ), float(tileMapDispl.y + posPlayer.y + 10)), velocitat);
		}
		else if (angle == UP_R)
		{
			up = true;
			velocitat.x = 2;
			velocitat.y = -2;
			bales[actualBullet]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 9), float(tileMapDispl.y + posPlayer.y+1)), velocitat);

		}
		else if (angle == UP_L)
		{
			up = true;
			velocitat.x = -2;
			velocitat.y = -2;
			bales[actualBullet]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x+3), float(tileMapDispl.y + posPlayer.y+1)), velocitat);

		}
		else if (angle == DOWN_R)
		{
			down = true;
			velocitat.x = 2;
			velocitat.y = 2;
			bales[actualBullet]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 12), float(tileMapDispl.y + posPlayer.y + 13)), velocitat);
		}
		else if (angle == DOWN_L)
		{
			down = true;
			velocitat.x = -2;
			velocitat.y = 2;
			bales[actualBullet]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y + 13)), velocitat);
		}
		else
		{
			up = true;
			velocitat.y = -2;
			bales[actualBullet]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 7), float(tileMapDispl.y + posPlayer.y + 13)), velocitat);
		}
		canShoot = 0;
		actualBullet += 1;

		if (tripleshoot)
		{
			glm::vec2 actualPos = glm::vec2(bales[actualBullet - 1]->getPositionX(), bales[actualBullet - 1]->getPositionY());
			int extraBullets = rand() % 4 + 1;
			for (int i = 0; i < extraBullets; i++)
			{
				glm::ivec2 velocitat2 = velocitat;
				if (up)
				{
					velocitat2.y -= (rand() % 3) - 1;
				}
				else if (down)
				{
					velocitat2.y += (rand() % 3) - 1;
				}
				else
				{
					if ((rand() % 2) == 1)
						velocitat2.y -= (rand() % 3);
					else
						velocitat2.y += (rand() % 3);
				}
				bales[actualBullet + i]->setPosition(actualPos, velocitat2);
			}
			actualBullet += extraBullets;
		}
	}
	



	
}

void Player::setFrontal(bool b) {
	frontal = b;
}

void Player::setRunUpAnimation() {
	runUp = true;
}

int Player::getRound() {
	return round;
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

glm::vec2 Player::getSpriteSize()
{
	return spriteSize;
}