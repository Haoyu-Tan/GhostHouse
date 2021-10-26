#ifndef KEY_INCLUDED
#define KEY_INCLUDED

//#include "Door.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/glm.hpp"

class Key {
public:
	int pos_x, pos_y;
	char index;
	glm::vec3 color;

	glm::vec3 center;
	glm::vec3 playerC;
	float size;
	float angle;
	float sf;

	glm::mat4 modelm;
	float pickupdist;
	bool picked;

	Key();
	Key(int p_x, int p_y, char i, float scaleFactor);
	~Key();

	//
	void update(float dt);
	void UpdateByPlayer(glm::vec3 ppos, glm::vec3 pf, glm::vec3 pup, glm::vec3 nright,int index, float ry, float rx, glm::vec3 facing,float dt);
	void SetCenter(glm::vec3 c);
	glm::mat4 GetModelMatrix();
	bool PickUp(glm::vec3 pos);
};

#endif
