#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Sphere
{
public:
	void init(const glm::ivec2& pos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void damage();
	void setPosition(const glm::vec2& pos);
	bool isDead();
	glm::vec2 getSpriteSize();
	glm::vec2 getPos();

private:
	glm::ivec2 pos, spriteSize;
	Texture spritesheet;
	Sprite* sprite;
	int hp;
};

