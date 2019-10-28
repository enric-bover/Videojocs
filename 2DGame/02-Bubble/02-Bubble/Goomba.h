#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Goomba
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
	void damage();
	glm::vec2 getSpriteSize();
	 

private:
	int jumpAngle, startY;
	Texture spritesheet2;
	
protected:
	glm::ivec2 tileMapDispl, posPlayer, spriteSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool bJumping;
	int hp;
	bool dead;
	int direction;

};


#endif // _PLAYER_INCLUDE

