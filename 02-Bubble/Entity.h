#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Entity
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgra, int entity);

	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	//glm::ivec2 getPosition();
	void setHideBalloon();
	int getHideBalloon();
	void transformChest();
	void eliminar();

	bool collisionEntity(const glm::ivec2& pos, const glm::ivec2& size) const;

private:
	glm::ivec2 tileMapDispl, posEntity, size;
	Texture spritesheet;
	Sprite* sprite;//, sprite2, sprite3;
	TileMap* map;
	bool up;
	int currentDisp, hideBalloon;
	bool elim;//, transformChest;
	int type;
};


#endif // _ENTITY_INCLUDE

