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
	void init_player();
	void init_entities();
	void update_map(int state);
	void update(int deltaTime, int state, bool* transition);
	//void update(int deltaTime, int state);
	void updateEntities(int deltaTime);
	void render(int state);
	void re_init_credits();
	void resetPlayer();
	void setVulnerability(bool v);
	bool getVulnerability();
	void setInfinity_dash(bool d);
	bool getInfinity_dash();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	glm::ivec2 posPlayer;
	std::vector<Entity*> entity;
	std::vector<int> entities;
	ShaderProgram texProgram;
	float currentTime;
	int jumpAngle, startY, shake, creditant;
	glm::mat4 projection;
	bool dead, vulnerability;

	Texture spritesheet_pantalla;
	Texture spritesheet_credits;
	Sprite* sprite_pantalla;
	Sprite* sprite_credits;
	int num;
};


#endif // _SCENE_INCLUDE

