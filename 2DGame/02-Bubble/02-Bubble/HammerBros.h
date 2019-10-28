#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Balas.h"
#include "Goomba.h"

class HammerBros : public Goomba
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	//void render();

	//bool dies(const glm::vec2& posEnemie, const glm::ivec2& sizeTile);
	//bool kills(const glm::vec2& posEnemie, const glm::ivec2& sizeTile);

private:
	//void shoot(const glm::vec2& pos, int angle);

private:
	int actualHammer;
	Balas* bales[90];
	int canShoot;

};

