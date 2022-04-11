#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0//64
#define SCREEN_Y 0//16

#define INIT_PLAYER_X_TILES 2//2//0
#define INIT_PLAYER_Y_TILES 14//14//11


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/level11.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	posPlayer = map->getInitialPos();
	player->setPosition(glm::vec2(posPlayer.x * map->getTileSize(), posPlayer.y * map->getTileSize()));
	player->setTileMap(map);
	dead = false;

	entities = map->getEntities();
	std::vector<int> positionx = map->getPosx();
	std::vector<int> positiony = map->getPosy();
	Entity *e;
	for (unsigned int i = 0;i < entities.size();i++) {
		e = new Entity();
		e->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, entities[i]);
		e->setPosition(glm::vec2(positionx[i] * map->getTileSize(), positiony[i] * map->getTileSize()));
		e->setTileMap(map);
		entity.push_back(e);
		/*entities[i] = new Entity();
		entity[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, entities[i]);
		entity[i]->setPosition(glm::vec2(positionx[i] * map->getTileSize(), positiony[i] * map->getTileSize()));
		entity[i]->setTileMap(map);*/
	}
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime, int state)
{
	if (state == 1 && !dead) {
		currentTime += deltaTime;
		player->update(deltaTime);
		updateEntities(deltaTime);
		
		/*
		entity[1]->update(deltaTime,1);
		entity[2]->update(deltaTime, 2);
		entity[3]->update(deltaTime, 3);
		if (entity[1]->collisionEntity(pos, glm::vec2(28, 28))) {
			player->increaseScore();
			entity[1]->eliminar();
			entity[2]->eliminar();
			entity[3]->eliminar();
		}
		entity[4]->update(deltaTime, 4);
		if (entity[4]->collisionEntity(pos, glm::vec2(28, 28))) {
			entity[4]->eliminar();
			types[5] = 1;
			entity[5]->transformChest();
		}
		entity[5]->update(deltaTime, types[5]);
		if (types[5] == 1 && entity[5]->collisionEntity(pos, glm::vec2(28, 28))) {
			player->increaseScore();
			entity[5]->eliminar();
		}*/
		//int i = 0;
		//for (;types[i] == 0 && !dead;i++) {
			/*glm::ivec2 pos1 = entity[i]->getPosition();
			dead = map->diesDowm(pos, glm::vec2(28,28), &pos.y, pos1, glm::vec2(32, 32));*/
		//	dead = entity[i]->collisionEntity(pos, glm::vec2(28, 28));
		//}
		/*
		if (num == 8 && entity[7]->collisionEntity(pos, glm::vec2(28, 28))) {
			player->increaseScore();
			delete entity[7];
			//num--;
		}
		for (;i < num;i++) {
			entity[i]->update(deltaTime, types[i]);
			if (i == 12) {
				bool newJump = entity[12]->collisionEntity(pos, glm::vec2(28, 28));
				if (newJump && entity[12]->getHideBalloon() == 0) {
					entity[12]->setHideBalloon();
					player->resetJump();
				}
			}
		}*/
	}
	else if (dead) {
		//sprite->update(deltaTime);
		player->setPosition(glm::vec2(posPlayer.x * map->getTileSize(), posPlayer.y * map->getTileSize()));
		player->update(deltaTime);
		dead = false;
	}
}

void Scene::updateEntities(int deltaTime) 
{
	glm::ivec2 pos = player->getPosition();
	for (unsigned int i = 0;i < entity.size();i++) {
		entity[i]->update(deltaTime);
		switch (entities[i]) {
		case 0:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28)) && !dead) {
				dead = true;
			}
			break;
		case 1:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28))) {
				player->increaseScore();
				entity[i]->eliminar();
				if (entities[i + 1] == 2) {
					entity[i + 1]->update(deltaTime);
					entity[i + 1]->eliminar();
					entity[i - 1]->eliminar();
					i += 1;
				}
				/*player->increaseScore();
				entity[i]->eliminar();
				if (types[i + 1] == 2) {
					entity[i + 1]->update(deltaTime);
					entity[i + 2]->update(deltaTime);
					entity[i + 1]->eliminar();
					entity[i + 2]->eliminar();
					i += 2;
				}*/
			}
			break;
		case 4:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28))) {
				entity[i]->eliminar();
				//entity[i+1]->transformChest();
				unsigned int e = 0;
				for (;e < entities.size() && entities[e] != 5;e++) {}
				entity[e]->transformChest();
			}
			break;
		case 5:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28))) {
				player->increaseScore();
				entity[i]->eliminar();
			}
			break;
		case 6:
			if (entity[i]->collisionEntity(pos, glm::vec2(28, 28)) && entity[i]->getHideBalloon() == 0) {
				entity[i]->setHideBalloon();
				player->resetJump();
			}
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		default:
			break;
		}
	}
}

void Scene::render(int state)
{
	if (state == 1) {
		glm::mat4 modelview;

		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		map->render();
		//if (!dead)
			player->render();
	
		for (unsigned int i = 0;i < entity.size();i++) entity[i]->render();
	}
	else {
		//sprite->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



