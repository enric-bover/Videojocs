#include "Explosion.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

enum ExplosionAnimation
{
	EXPLOSION
};


void Explosion::init(const glm::ivec2& pos, ShaderProgram& shaderProgram)
{
	engine = createIrrKlangDevice();
	end = true;
	spriteSize = glm::ivec2(32, 32);
	spritesheet.loadFromFile("images/explosion.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(spriteSize, glm::vec2(1.0f/7.0f, 1.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(EXPLOSION, 7);
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(1.0f / 7.0f, 0.0f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(2.0f / 7.0f, 0.0f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(3.0f / 7.0f, 0.0f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(4.0f / 7.0f, 0.0f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(5.0f / 7.0f, 0.0f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(6.0f / 7.0f, 0.0f));

	sprite->changeAnimation(EXPLOSION);
	this->pos = pos;
	sprite->setPosition(pos);

}

void Explosion::update(int deltaTime)
{
	if (sprite->lastKeyFrame())
		end = true;
	else if (!end)
		sprite->update(deltaTime);
}

void Explosion::render()
{
	if (!end)
		sprite->render();
}

void Explosion::setPosition(const glm::vec2& pos)
{
	this->pos = pos;
	sprite->setPosition(pos);
}

glm::vec2 Explosion::getSpriteSize()
{
	return spriteSize;
}

glm::vec2 Explosion::getPos()
{
	return pos;
}

void Explosion::start() {
	engine->play2D("music/explosion.mp3");
	end = false;
}

bool Explosion::hasFinished() {
	return end;
}