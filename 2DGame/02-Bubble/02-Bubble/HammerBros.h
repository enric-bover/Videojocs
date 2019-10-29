#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Balas.h"
#include "Goomba.h"

class HammerBros : public Goomba
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, glm::ivec2 posEnemy);
	void render(float cameraX, float cameraWidht);

	//bool dies(const glm::vec2& posEnemie, const glm::ivec2& sizeTile);
	//bool kills(const glm::vec2& posEnemie, const glm::ivec2& sizeTile);
	void setMode(int mode);
	bool collisionWithHammer(glm::ivec2 pos, glm::ivec2 size);

private:
	void shoot(const glm::vec2& pos, glm::ivec2 posEnemy);

private:
	int actualHammer;
	Balas* bales[90];
	int canShoot;
	bool frontal;
	int actionTimer;

};

