#ifndef _BLOCK_INTERROGANTE_INCLUDE
#define _BLOCK_INTERROGANTE_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "spreadGunBuff.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class block_interrogante
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	int getPositionX();
	int getPositionY();
	bool isDead();
	bool BuffisActive();
	void damage();
	void start();
	void BuffDamage();
	glm::vec2 getSpriteSize();
	glm::vec2 getBuffSpriteSize();
	int getBuffPositionX();
	int getBuffPositionY();


private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer, spriteSize;
	int jumpAngle, startY;
	Texture spritesheet;
	Texture spritesheet2;
	Sprite* sprite;
	TileMap* map;
	int hp;
	int direction;
	bool dead;
	spreadGunBuff* buff;
};

#endif