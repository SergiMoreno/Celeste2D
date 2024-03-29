#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Entity.h"
#include "Text.h"

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
	void shaking(int *shake, int *posPlayerx, int *posPlayery);

	static Scene& instance()
	{
		static Scene S;

		return S;
	}

	int state;

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	glm::ivec2 posPlayer, initPos;
	std::vector<Entity*> entity;
	std::vector<int> entities;
	ShaderProgram texProgram;
	float currentTime;
	int jumpAngle, startY, shake, creditant, showScore;
	glm::mat4 projection;
	bool dead, vulnerability;
	Text text;

	Texture spritesheet_pantalla;
	Texture spritesheet_credits;
	Texture spritesheet_snow1;
	Texture spritesheet_snow2;
	Texture spritesheet_snow3;
	Texture spritesheet_snow4;
	Texture spritesheet_pols1;
	Texture spritesheet_pols2;
	Texture spritesheet_pols3;
	Texture spritesheet_pols4;
	Sprite* sprite_pantalla;
	Sprite* sprite_credits;
	Sprite* sprite_snow1;
	Sprite* sprite_snow2;
	Sprite* sprite_snow3;
	Sprite* sprite_snow4;
	Sprite* sprite_pols1;
	Sprite* sprite_pols2;
	Sprite* sprite_pols3;
	Sprite* sprite_pols4;
	int num;
};


#endif // _SCENE_INCLUDE

