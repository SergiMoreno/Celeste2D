#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Entity.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime, int state);
	void render(int state);

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Entity *entity[14];
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool dead;

	//Texture spritesheet;
	//Sprite* sprite;
	//int types[14] = {0,0,0,0,0,0,0,1,2,3,4,5,6,7};
	int types[6] = { 0,1,2,3,4,5};
	int num;
};


#endif // _SCENE_INCLUDE

