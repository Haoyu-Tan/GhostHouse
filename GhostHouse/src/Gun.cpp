#include "../include/Gun.h"

Gun::Gun(int x, int y, float scaleFactor,DModel* d) {
	animeSI = d->animations.at(0)->frames.at(0)->startIndex;
	totalVert = d->animations.at(0)->frames.at(0)->totalVert;
	sf = scaleFactor;
	position = glm::vec3(-x * sf, 1.0f, y * sf);
	angle = 0.0f;
	size = 2.0f;
}

Gun::~Gun() {
}

void Gun::Update(float dt) {
	float Pi = 3.14159265358979323846;
	float angleS = 90;
	angle = angle + dt * angleS;
	if (angle > 360) angle = angle - 360;

	modelm = glm::mat4(1);
	modelm = glm::translate(modelm, position);
	modelm = glm::scale(modelm, glm::vec3(size, size, size));
	modelm = glm::rotate(modelm, angle * Pi / 180, glm::vec3(0.0f, 1.0f, 0.0f));
}

bool Gun::PickUp(glm::vec3 pos) {
	float pickupdist = 1.0f;
	glm::vec3 dir = pos - position;
	dir.y = 0.0f;
	float dist = glm::length(dir);
	if (dist <= pickupdist) {
		return true;
	}
	return false;
}