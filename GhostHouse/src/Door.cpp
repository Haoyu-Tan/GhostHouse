#include "../include/Door.h"
#include "../glm/glm.hpp"

#include <vector>

Door::Door() {
	pos_x = 0;
	pos_y = 0;
	index = ' ';
	color = glm::vec3(0, 0, 0);
	isOpen = false;

	top = 0.0;
	bottom = 0.0;
	left = 0.0;
	right = 0.0;
	currentHeight = highest;
	freezeTime = max_freeze_time;
	status = 0;
	scalefac = 1.0;
	
}

Door::Door(int p_x, int p_y, char i, float sf,int type) {
	pos_x = p_x;
	pos_y = p_y;
	index = i;
	color = glm::vec3(0, 0, 0);
	isOpen = false;
	float outside = 0.2;
	float centerx = -p_x * sf;
	float centery = p_y * sf;
	float Pi = 3.14159265358979323846;
	//float factorx = 1.0;
	//float factory = 1.0;
	if (type == 0) {
		angle = 0.0f;
		//factory = 0.5f;
		
	}
	else {
		angle = 0.5 * Pi;
		//factorx = 0.5f;
	}
	top = centery +  float(sf) / 2.0f + outside;
	bottom = centery -  float(sf) / 2.0f - outside;
	left = centerx -  float(sf) / 2.0f - outside;
	right = centerx +  float(sf) / 2.0f + outside;
	printf("TOP is %f, Bottom is %f, left is %f, right is %f", top, bottom, left, right);
	
	
	freezeTime = max_freeze_time;
	status = 0;
	scalefac = sf;
	currentHeight = highest;
	position = glm::vec3(-pos_x * scalefac, 0.0f, pos_y * scalefac);

}

bool Door::CheckPosition(float l, float r, float t, float b) {
	if (l > left && l < right && t < top && t > bottom) return true;
	if (l > left && l < right && b < top && b > bottom) return true;
	if (r > left && r < right && t < top && t > bottom) return true;
	if (r > left && r < right && b < top && b > bottom) return true;
	if (left > l && left < r && top < t && top > b) return true;
	if (left > l && left < r && bottom < t && bottom > b) return true;
	if (right > l && right < r && top < t && top > b) return true;
	if (right > l && right < r && bottom < t && bottom > b) return true;

	return false;
}

void Door::checkPP(glm::vec3 pp) {
	if (status != 0) return;
	glm::vec3 dir = pp - position;
	dir.y = 0;
	float len = glm::length(dir);

	printf("len is: %f\n", len);
	//check dist
	if (len - 2.8f <= 0.001f) {
		status = 1;
	}
	/*
	else if (status == 2) {
		if (keys.size() > 0) {
			if (len - 2.0f <= 0.001f) {
				status = 3;
			}
		}
	}*/
	printf("status: %d\n", status);
}

void Door::update(float dt, glm::vec3 pp) {
	checkPP(pp);

	switch (status) {
		case 0:
			//pending
			currentHeight = highest;
			break;
		case 1:
			//falling
			if (currentHeight > 0.0) {
				//currentHeight -= 6.5 * speed * dt;
				currentHeight -= 10.0 * speed * dt;
			}
			else {
				currentHeight = 0.0;
				status = 2;
			}
			break;
		case 2:
			//falling->static
			currentHeight = 0.0;
			break;
		case 3:
			//static->up
			if (highest - currentHeight > 0) {
				currentHeight += speed * dt;
			}
			else {
				currentHeight = highest;
				status = -1;
			}
			break;
	}
	
}

/* 
void Door::update(float dt, glm::vec3 pp) {
	printf("pp: %f, %f, %f\n", pp.x, pp.y, pp.z);
	printf("dp: %f, %f\n", pos_x * scalefac, pos_y * scalefac);
	if (currentHeight < highest && status == 0) {
		currentHeight += dt * speed;
	}
	else if (currentHeight <= 0) {
		status = 0;
	}
	else if (currentHeight < highest && status == 1) {
		currentHeight -= dt * 10 * speed;
	}
	
	else if (currentHeight >= highest) {
		freezeTime -= dt;
		if (freezeTime <= 0.0) {
			status = 1;
			currentHeight -= 0.01;
		}
	}
	
}
*/