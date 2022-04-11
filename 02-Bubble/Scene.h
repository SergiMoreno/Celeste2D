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
	void update_map(int state);
	void update(int deltaTime, int state, bool* transition);
	//void update(int deltaTime, int state);
	void updateEntities(int deltaTime);
	void render(int state);
	void re_init_credits();
	void resetPlayer();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	//Entity *entity[20];
	glm::ivec2 posPlayer;
	std::vector<Entity*> entity;
	std::vector<int> entities;
	//int *entities;
	ShaderProgram texProgram;
	float currentTime;
	int jumpAngle, startY, shake, creditant;
	glm::mat4 projection;
	bool dead;

	Texture spritesheet_pantalla;
	Texture spritesheet_credits;
	Sprite* sprite_pantalla;
	Sprite* sprite_credits;
	//Texture spritesheet;
	//Sprite* sprite;
	//int types[14] = {0,0,0,0,0,0,0,1,2,3,4,5,6,7};
	int types[13] = { 0,1,2,3,4,5,6,7,8,9,10,11,12};
	int num;
};


#endif // _SCENE_INCLUDE

