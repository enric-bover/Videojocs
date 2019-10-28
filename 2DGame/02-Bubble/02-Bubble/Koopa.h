#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Koopa
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

	ISoundEngine* engine;

};


#endif // _PLAYER_INCLUDE

