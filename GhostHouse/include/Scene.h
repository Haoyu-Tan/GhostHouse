//Map.h
#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <vector>
#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

//#include "Wall.h"
#include "Door.h"
#include "fruit.h"
#include "MyStruct.h"
//#include "StaticFurniture.h"
#include "FBullet.h"
#include "Gun.h"
#include "PGun.h"
#include "Candel.h"

class Scene {
public:
	int width, height;
	std::vector<Wall*> walls;
	std::vector<Door*> doors;
	std::vector<Key*> keys;
	std::vector<Key*> pKeys; // key belongs to player
	std::vector<Fruit*> fruits;
	//std::vector<Key*> currentKeys;
	std::vector<glm::vec3> colors;
	std::vector<StaticFurniture*> staticFurnitures;
	std::vector<WhiteGhost*> whiteGhosts;
	std::vector<BlueGhost*> blueGhosts;
	std::vector<UglyBall*> uglyBalls;
	std::vector<Gun*> guns;
	std::vector<FBullet*> fbuls;
	std::vector<Candel*> candels;
	PGun* pgun;

	float floorHeight;

	int start_x, start_y;
	int goal_x, goal_y;

	float scaleFactor;
	int gameState; //0 in game, 1 game over

	HealthBar* healthBar;

	Scene();
	Scene(int w, int h, int s_x, int s_y, int g_x, int g_y);
	~Scene();
	glm::vec3 StartPosition();
	void printSceneInfo();
	void drawDGeometry(int shaderProgram, GLuint vao, GLuint vbo);
	void drawGeometry(int shaderProgram, std::vector<Model*> staticModels,
		float timePast, float scaleFactor, GLuint vao, GLuint vbo[]);
	void Update(float dt, glm::vec3 pp, glm::vec3 pd);
};

#endif