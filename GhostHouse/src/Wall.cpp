#include "../include/Wall.h"

Wall::Wall() {
	pos_x = 0;
	pos_y = 0;
	top = 0.0;
	bottom = 0.0;
	left = 0.0;
	right = 0.0;
}

Wall::Wall(int p_x, int p_y, float sf, glm::vec3 ra,glm::vec3 s) {
	pos_x = p_x;
	pos_y = p_y;
	scale = s;
	position = glm::vec3(-sf * pos_x, 0, sf * pos_y-0.5*sf);
	float outside = 0.2;
	float centerx = -p_x * sf;
	float centery = p_y * sf;
	top = centery + sf / 2 + outside;
	bottom = centery - sf / 2 - outside;
	left = centerx - sf / 2 - outside;
	right = centerx + sf / 2 + outside;
	rotateAngle = ra;
}

bool Wall::CheckPosition(float l, float r, float t, float b) {
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