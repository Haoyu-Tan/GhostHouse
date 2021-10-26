#include "../include/fruit.h"
#include "../glm/glm.hpp"

Fruit::Fruit() {
	pos_x = 0;
	pos_y = 0;

	color = glm::vec3(0.4f, 0.3f, 0.5f);

	//size = 0.25;
	//angle = 0.0;
	center = glm::vec3(0, 2, 0);
	sf = 0.0;
	//modelm = glm::mat4(1);
	picked = false;
	//effectiveTime = 5.0;
}
Fruit::Fruit(int p_x, int p_y, float scaleFactor) {
	pos_x = p_x;
	pos_y = p_y;

	color = glm::vec3(0.4f, 0.3f, 0.5f);

	pickupdist = 1;
	//angle = 0.0;
	//size = 0.25;
	sf = scaleFactor;
	center = glm::vec3(-pos_x * sf, 1.0, pos_y * sf);
	picked = false;
	//effectiveTime = 5.0;
}
Fruit::~Fruit() {
}

//

bool Fruit::PickUp(glm::vec3 pos) {
	if (picked) return false;
	float dist = glm::length(pos - center);
	if (dist < pickupdist) {
		picked = true;
		return true;
	}
	return false;
}
