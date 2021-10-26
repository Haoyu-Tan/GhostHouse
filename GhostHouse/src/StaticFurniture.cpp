#include "../include/StaticFurniture.h"

StaticFurniture::StaticFurniture() {
	pos_x = 0;
	pos_y = 0;
	left = 0.0;
	right = 0.0;
	top = 0.0;
	bottom = 0.0;
	modelType = bookcase;
	rotateAngle = glm::vec3(0.0, 0.0, 0.0);
	ranslateDist = glm::vec3(0.0, 0.0, 0.0);
}

StaticFurniture::StaticFurniture(float p_x, float p_y, float sf, sModelType type, glm::vec3 ra, glm::vec3 tdist, int gtype) {
	
	float outside = 0.2;
	float factorx = 1;
	float factory = 1;
	if (gtype == 1) {
		factory = 0.5;
	}
	else if (gtype == 2) {
		factorx = 0.5;
	}

	pos_x = p_x;
	pos_y = p_y;
	float centerx = -p_x * sf;
	float centery = p_y * sf;
	top = centery + sf * factory / 2.0f + outside;
	bottom = centery - sf * factory / 2.0f - outside;
	left = centerx - sf * factorx / 2.0f - outside;
	right = centerx + sf * factorx / 2.0f + outside;

	top1 = centery + sf * factory / 2.0f;
	bottom1 = centery - sf * factory / 2.0f;
	left1 = centerx - sf * factorx / 2.0f;
	right1 = centerx + sf * factorx / 2.0f;

	modelType = type;
	rotateAngle = ra;
	ranslateDist = tdist;
}

StaticFurniture::~StaticFurniture() {

}

bool StaticFurniture::CheckPosition(float l, float r, float t, float b) {
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


bool StaticFurniture::CheckObjPosition(float l, float r, float t, float b) {
	if (l > left1 && l < right1 && t < top1 && t > bottom1) return true;
	if (l > left1 && l < right1 && b < top1 && b > bottom1) return true;
	if (r > left1 && r < right1 && t < top1 && t > bottom1) return true;
	if (r > left1 && r < right1 && b < top1 && b > bottom1) return true;
	if (left1 > l && left1 < r && top1 < t && top1 > b) return true;
	if (left1 > l && left1 < r && bottom1 < t && bottom1 > b) return true;
	if (right1 > l && right1 < r && top1 < t && top1 > b) return true;
	if (right1 > l && right1 < r && bottom1 < t && bottom1 > b) return true;

	return false;
}