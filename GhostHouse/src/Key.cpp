#include "../include/Key.h"
#include "../glm/glm.hpp"

Key::Key() {
	pos_x = 0;
	pos_y = 0;
	//myDoor = nullptr;
	index = ' ';
	color = glm::vec3(0, 0, 0);


	size = 0.25;
	angle = 0.0;
	center = glm::vec3(0.0f, 2.0f, 0.0f);
	sf = 0.0;
	modelm = glm::mat4(1);
}

Key::Key(int p_x, int p_y, char i, float scaleFactor) {
	pos_x = p_x;
	pos_y = p_y;
	//myDoor = d;
	index = i;
	color = glm::vec3(0, 0, 0);

	pickupdist = 1;
	angle = 0.0;
	size = 5;
	sf = scaleFactor;
	center = glm::vec3(-pos_x * sf, 1.0f, pos_y * sf);
	picked = false;

}

Key::~Key() {
}


void Key::UpdateByPlayer(glm::vec3 ppos, glm::vec3 pf, glm::vec3 pup,glm::vec3 nright, int index, float ry, float rx, glm::vec3 facing,float dt) {
	//glm::vec3 nright = glm::normalize(glm::cross(pf, pup));
	center = ppos + glm::normalize(pup) * 0.05f - nright * 0.07f + nright * 0.0125f * (float)(index);
	glm::vec3 pos1 = facing * 0.15f;

	glm::vec3 rotateAngle = glm::vec3(0.0, 0.0, 0.0);
	rotateAngle.y = rx;
	rotateAngle.x = ry;

	float angleS = 90;
	angle = angle + dt * angleS;
	if (angle > 360) angle = angle - 360;

	modelm = glm::mat4(1);
	modelm = glm::translate(modelm, center);
	modelm = glm::rotate(modelm, rotateAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelm = glm::rotate(modelm, rotateAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelm = glm::translate(modelm, pos1);
	modelm = glm::rotate(modelm, angle * 3.14f / 180, glm::vec3(0.0f, 1.0f, 1.0f));
	modelm = glm::scale(modelm, glm::vec3(size, size, size));
}


void Key::update(float dt) {
	//if (picked) {
	//	center = playerC;
		//center = player->GetPosition() + player->facing;
	//}
	float angleS = 90;
	angle = angle + dt * angleS;
	if (angle > 360) angle = angle - 360;
	modelm = glm::mat4(1);
	modelm = glm::translate(modelm, center);
	modelm = glm::scale(modelm, glm::vec3(size, size, size));
	modelm = glm::rotate(modelm, angle * 3.14f / 180, glm::vec3(0.0f, 1.0f, 1.0f));

}

bool Key::PickUp(glm::vec3 pos) {
	if (picked) return false;
	float dist = glm::length(pos - center);
	if (dist < pickupdist) {
		picked = true;
		size = 0.1;
		return true;
	}
	return false;
}

void Key::SetCenter(glm::vec3 c) {
	playerC = c;
}

glm::mat4 Key::GetModelMatrix() {
	return modelm;
}

