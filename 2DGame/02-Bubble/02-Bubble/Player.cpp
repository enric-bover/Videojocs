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



void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	dead = false; 
	right = true;
	chooseSprite = 1;
	actualBullet = 0;
	spritesheet.loadFromFile("images/tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet2.loadFromFile("images/ShootingStraight+DiagonalDown.png", TEXTURE_PIXEL_FORMAT_RGBA);

	spriteSize = glm::ivec2(20, 35);
	spriteSize2 = glm::ivec2(25, 35);

	for (int i = 0; i < 50; i++)
	{
		bales[i] = new Balas();
		bales[i]->init(tileMapPos, shaderProgram);
		bales[i]->setTileMap(map);
	}

	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.1, 0.33), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);
	
	sprite2 = Sprite::createSprite(spriteSize2, glm::vec2(0.1667, 0.5), &spritesheet2, &shaderProgram);
	sprite2->setNumberAnimations(8);

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

		
	sprite->changeAnimation(0);
	sprite2->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite2->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));


}

// --
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite2->update(deltaTime);

	if (actualBullet == 50) actualBullet = 0;
	if (!bJumping) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			right = false;
			if (Game::instance().getKey(' '))		//dispares
			{
				
				if (Game::instance().getKey('w'))
				{
					if (sprite->animation() != SHOOT_DI_U_L)
						sprite->changeAnimation(SHOOT_DI_U_L);

					chooseSprite = 1;
				}
				else if (Game::instance().getKey('s'))
				{
					if (sprite2->animation() != SHOOT_DI_D_L)
						sprite2->changeAnimation(SHOOT_DI_D_L);
					chooseSprite = 2;
				}
				else
				{
					
					if (sprite2->animation() != SHOOT_S_L)
						sprite2->changeAnimation(SHOOT_S_L);
					chooseSprite = 2;
				}
			}
			else
			{
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
				chooseSprite = 1;

			}
			posPlayer.x -= int(SPEED * deltaTime);
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(20, 35)))
			{
				posPlayer.x += int(SPEED * deltaTime);
				sprite->changeAnimation(STAND_LEFT);
				chooseSprite = 1;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			right = true;
			if (Game::instance().getKey(' '))		//dispares
			{
				if (Game::instance().getKey('w'))
				{
					if (sprite->animation() != SHOOT_DI_U_R)
						sprite->changeAnimation(SHOOT_DI_U_R);
					chooseSprite = 1;
				}
				else if (Game::instance().getKey('s'))
				{
					if (sprite2->animation() != SHOOT_DI_D_R)
						sprite2->changeAnimation(SHOOT_DI_D_R);
					chooseSprite = 2;
				}
				else
				{
					bales[actualBullet]->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + 15), float(tileMapDispl.y + posPlayer.y + 11)));
					actualBullet++;
					if (sprite2->animation() != SHOOT_S_R)
						sprite2->changeAnimation(SHOOT_S_R);
					chooseSprite = 2;
				}
			}
			else
			{
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
				chooseSprite = 1;

			}
			posPlayer.x += int(SPEED * deltaTime);
			if (map->collisionMoveRight(posPlayer, glm::ivec2(20, 35)))
			{
				posPlayer.x -= int(SPEED * deltaTime);
				sprite->changeAnimation(STAND_RIGHT);
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
						sprite->changeAnimation(STAND_SHOOT_DI_U_R);
						chooseSprite = 1;
					}
					else if (Game::instance().getKey('s'))
					{
						sprite2->changeAnimation(STAND_SHOOT_DI_D_R);
						chooseSprite = 2;
					}
					else 
					{
						sprite2->changeAnimation(SHOOT_STAND_S_R);
						chooseSprite = 2;
					}
					right = true;
				}
				else
				{
					if (Game::instance().getKey('w'))
					{
						sprite->changeAnimation(STAND_SHOOT_DI_U_L);
						chooseSprite = 1;
					}
					else if (Game::instance().getKey('s'))
					{
						sprite2->changeAnimation(STAND_SHOOT_DI_D_L);
						chooseSprite = 2;
					}
					else
					{
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
	

	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite2->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	for (int i = 0; i < 50; i++)
	{
		if (bales[i]->isAlive())
			bales[i]->update(deltaTime);
	}
}

void Player::render()
{
	if(chooseSprite == 1)
		sprite->render();
	else
		sprite2->render();
	for (int i = 0; i < 50; i++)
	{
		if (bales[i]->isAlive())
		{
			bales[i]->render();
		}

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
	//hago el tamaño de colision mas pequeño para que no se note tanto que tenemos pixeles vacios delante del sprite del player
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
	for (int i = 0; i < 10; i++) 
	{
		if (bales[i]->dies(posEnemie, sizeTile))
		{
			return true;
		}
	}
	return false;
}


/*
			if (chooseSprite == 1)
				{
					if (Game::instance().getKey('w'))
					{
						if (sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_LEFT  || sprite->animation() == STAND_SHOOT_DI_U_L || sprite->animation() == STAND_LEFT || sprite->animation() == SHOOT_DI_U_L)
							sprite->changeAnimation(STAND_SHOOT_DI_U_L);
						else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMP_RIGHT  || sprite->animation() == STAND_SHOOT_DI_U_R || sprite->animation() == STAND_RIGHT || sprite->animation() == SHOOT_DI_U_R)
							sprite->changeAnimation(STAND_SHOOT_DI_U_R);
						chooseSprite = 1;
					}
					else if (Game::instance().getKey('s'))
					{
						if (sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_LEFT || sprite->animation() == STAND_SHOOT_DI_U_L || sprite->animation() == STAND_LEFT)
							sprite2->changeAnimation(STAND_SHOOT_DI_D_L);
						else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == STAND_SHOOT_DI_U_R || sprite->animation() == STAND_RIGHT)
							sprite2->changeAnimation(STAND_SHOOT_DI_D_R);
						chooseSprite = 2;
					}
					else
					{
						if (sprite->animation() == MOVE_LEFT || sprite->animation() == JUMP_LEFT || sprite->animation() == STAND_SHOOT_DI_U_L || sprite->animation() == STAND_LEFT)
							sprite2->changeAnimation(SHOOT_STAND_S_L);
						else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == STAND_SHOOT_DI_U_R || sprite->animation() == STAND_RIGHT)
							sprite2->changeAnimation(SHOOT_STAND_S_R);
						chooseSprite = 2;
					}
				}
				else if (chooseSprite == 2)
				{
					if (sprite2->animation() == SHOOT_DI_D_L || sprite2->animation() == SHOOT_S_L)
						sprite2->changeAnimation(SHOOT_STAND_S_L);
					if (sprite2->animation() == SHOOT_DI_D_R || sprite2->animation() == SHOOT_S_R)
						sprite2->changeAnimation(SHOOT_STAND_S_R);

				}
				chooseSprite = 2;
*/