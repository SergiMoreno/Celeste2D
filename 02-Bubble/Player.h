#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
//#include "Entity.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	void resetJump();
	void increaseScore();

	bool looksLeft();
	bool looksRight();
	
private:
	bool bJumping, bDashing, cReleased, sPressed ,fromWall, bMoving, bClimbing;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, dimyPlayer, dimxPlayer, direction, dashing_count, dash_direction, walking, climbing, wallAngle, otherY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int score;
	bool arriba;
	int posInicial;
	int transicion;
	int slowmo;
	//int JUMP_ANGLE_STEP, FALL_STEP, MOVIMENT_LATERAL;
};


#endif // _PLAYER_INCLUDE


