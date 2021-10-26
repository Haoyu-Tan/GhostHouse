#ifndef DOOR_INCLUDED
#define DOOR_INCLUDED

#include "../glm/glm.hpp"
#include "Key.h"
#include <vector>

class Door {
public:
	int pos_x, pos_y;

	glm::vec3 position;
	char index;
	glm::vec3 color;
	bool isOpen;
	float left, right, top, bottom;
	const float highest = 5;
	const float speed = 1.5;
	float currentHeight;
	float angle;
	float scalefac;
	float freezeTime;
	const float max_freeze_time = 2.0;
	int status; //0:pending; 1:down 2:down-static 3:up 4:up-pending

	Door();
	Door(int p_x, int p_y, char i, float sf, int type);
	void checkPP(glm::vec3 pp);
	bool CheckPosition(float l, float r, float t, float b);
	void update(float dt, glm::vec3 pp);
	
};

#endif
