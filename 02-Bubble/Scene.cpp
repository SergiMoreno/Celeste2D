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

	spritesheet_credits.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite_credits = Sprite::createSprite(glm::ivec2(512, 1024), glm::vec2(1, 1), &spritesheet_credits, &texProgram);


	spritesheet_pantalla.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite_pantalla = Sprite::createSprite(glm::ivec2(512, 512), glm::vec2(1, 1), &spritesheet_pantalla, &texProgram);
	sprite_pantalla->setPosition(glm::vec2(0, 0));


	/*spritesheet.loadFromFile("images/title.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &texProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(0.0, 0.0));*/

	map = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//map = TileMap::createTileMap("menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	dead = false;

	//int vecPosx[14] = {5,6,9,10,11,12,13,8,9,11,   8,13,2,2};
	//int vecPosy[14] = {14,14,13,13,12,12,12,9,6,6,  10,2,10,11};
	num = 6;
	int vecPosx[6] = { 4,8,7,9,12,14};
	int vecPosy[6] = { 14,14,14,14,14,14};
	/*
	for (int i = 0;i < num;i++) {
		entity[i] = new Entity();
		entity[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, types[i]);
		if (i == 2) entity[i]->setPosition(glm::vec2(vecPosx[i] * map->getTileSize() + 4, vecPosy[i] * map->getTileSize()));
		else if (i == 3) entity[i]->setPosition(glm::vec2(vecPosx[i] * map->getTileSize() -4, vecPosy[i] * map->getTileSize()));
		else entity[i]->setPosition(glm::vec2(vecPosx[i] * map->getTileSize(), vecPosy[i] * map->getTileSize()));
		entity[i]->setTileMap(map);
	}*/
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	jumpAngle = 180;
	shake = 0;
	creditant = 0;
}

void Scene::update_map(int state) {
	if (state > 0 && state < 12) {
		if (state < 10) {
			map = TileMap::createTileMap("levels/level0" + to_string(state) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			player->setTileMap(map);
		}
		else {
			map = TileMap::createTileMap("levels/level" + to_string(state) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			player->setTileMap(map);
		}
	}
}

void Scene::update(int deltaTime, int state, bool *transition)
{
	if (*transition == false && !dead) {
		if (state > 0 && state < 12) {
			currentTime += deltaTime;
			player->update(deltaTime);
		}
		//currentTime += deltaTime;
		//player->update(deltaTime);
		//glm::ivec2 pos = player->getPosition();


		/*dead = entity[0]->collisionEntity(pos, glm::vec2(28, 28));
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
	else if (dead || *transition) {
		if (jumpAngle == 180)
		{
			jumpAngle = 0;
			startY = 512;
		}
		else
		{
			if (shake == 0) {
				jumpAngle += 4;

				posPlayer = glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), int(startY - 4 * 40 * sin(3.14159f * jumpAngle / 180.f)));
				player->setPosition(posPlayer);
			}
			if (jumpAngle > 90) {
				if ((map->collisionMoveDown(posPlayer, glm::ivec2(28, 28), &posPlayer.y)) || shake > 0) {
					switch (shake) {
					case 4:
						if (state < 10) {
							map = TileMap::createTileMap("levels/level0" + to_string(state) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
						}
						else {
							map = TileMap::createTileMap("levels/level" + to_string(state) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
						}
						posPlayer = glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), int(startY - 4 * 40 * sin(3.14159f * jumpAngle / 180.f)));
						*transition = false;
						dead = false;
						jumpAngle = 180;
						shake = 0;
						break;
					default:
						if (state < 10) {
							map = TileMap::createTileMap("levels/level0" + to_string(state) + ".txt", glm::vec2(((shake % 2) * 2 - 1) * 5, (((shake + 1) % 2) * 2 - 1) * 5), texProgram);
						}
						else {
							map = TileMap::createTileMap("levels/level" + to_string(state) + ".txt", glm::vec2(((shake % 2) * 2 - 1) * 5, (((shake + 1) % 2) * 2 - 1) * 5), texProgram);
						}
						posPlayer = glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + ((shake % 2) * 2 - 1) * 5, int(startY - 4 * 40 * sin(3.14159f * jumpAngle / 180.f)) + (((shake + 1) % 2) * 2 - 1) * 5);
						++shake;
						break;
					}
					player->setPosition(posPlayer);
				}
			}
		}
	}
}

void Scene::render(int state)
{
	/*if (state == 1) {
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
		int i = 0;
		//for (;i < num;i++) entity[i]->render();
	}
	else {
		//sprite->render();
	}*/

	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	switch (state) {
	case 0:			//Title
		spritesheet_pantalla.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite_pantalla->render();
		break;
	case 13:		//Instructions
		spritesheet_pantalla.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite_pantalla->render();
		break;
	case 12:		//Credits
		if (creditant > -512) {
			--creditant;
			sprite_credits->setPosition(glm::vec2(0, creditant));
		}
		sprite_credits->render();
		break;
	default:
		map->render();
		player->render();
		break;
	}
}

void Scene::re_init_credits() {
	creditant = 0;
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

void Scene::resetPlayer() {
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
}




