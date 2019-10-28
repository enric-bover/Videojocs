#include "Sphere.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

enum SphereAnimation
{
	PARPADEO
};


void Sphere::init(const glm::ivec2& pos, ShaderProgram& shaderProgram)
{
	hp = 20;
	spriteSize = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/sphere.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(0.5, 1.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(PARPADEO, 8);
	sprite->addKeyframe(PARPADEO, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(PARPADEO, glm::vec2(0.5f, 0.0f));

	sprite->changeAnimation(PARPADEO);
	this->pos = pos;
	sprite->setPosition(pos);
}

void Sphere::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Sphere::render()
{
	if (hp > 0)
		sprite->render();
}

void Sphere::damage()
{
	hp--;
}

void Sphere::setPosition(const glm::vec2& pos)
{
	this->pos = pos;
	sprite->setPosition(pos);
}
bool Sphere::isDead()
{
	return hp <= 0;
}

glm::vec2 Sphere::getSpriteSize()
{
	return spriteSize;
}

glm::vec2 Sphere::getPos()
{
	return pos;
}