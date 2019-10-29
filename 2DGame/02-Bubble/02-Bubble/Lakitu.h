#ifndef _LAKITU_INCLUDE
#define _LAKITU_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Spinny.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Lakitu
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram/*, const glm::ivec2& MinMaxX*/);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	int getPositionX();
	int getPositionY();
	bool isDead();
	void damage();
	void setMaxMinX(const glm::vec2& pos);
	bool activeEnemies();
	glm::vec2 getSpriteSize();

private:
	int livingSpinny();
private:
	bool bJumping;
	bool throwSpinny;
	int countThrow;
	glm::ivec2 tileMapDispl, posPlayer, spriteSize;
	int jumpAngle, startY;
	int MinX, MaxX;
	Spinny* spinny[5];
	Texture spritesheet;
	Texture spritesheet2;
	Sprite* sprite;
	TileMap* map;
	int hp;
	int direction;
	int movementy;
	bool dead;

};


#endif // _PLAYER_INCLUDE

