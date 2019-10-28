#pragma once

#include "Sprite.h"
#include "TileMap.h"

#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

class Explosion
{
public:
	void init(const glm::ivec2& pos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2& pos);
	glm::vec2 getSpriteSize();
	glm::vec2 getPos();
	void start();
	bool hasFinished();

private:
	glm::ivec2 pos, spriteSize;
	Texture spritesheet;
	Sprite* sprite;
	bool end;

	ISoundEngine* engine;
};

