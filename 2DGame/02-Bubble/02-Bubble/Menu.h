#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "TexturedQuad.h"
#include "ShaderProgram.h"

class Menu
{

public:
	Menu();
	~Menu();

	void init();
	int update(int deltaTime);
	void render();

private:
	void initShaders();
	void updatePosPoint();

private:
	Texture image[3], imageCursor;
	TexturedQuad* background, *cursor;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int selectedOption, currentScreen;
	glm::vec2 posPoint;
	float timer;
	bool startTimer;
};


#endif // _MENU_INCLUDE

