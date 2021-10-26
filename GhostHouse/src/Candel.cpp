#include "../include/Candel.h"


Candel::Candel(float p_x, float p_y, float sf, sModelType type, glm::vec3 ra, glm::vec3 tdist, int gtype) {
	
	float factorx = 1;
	float factory = 1;
	if (gtype == 1) {
		factory = 0.5;
	}
	else if (gtype == 2) {
		factorx = 0.5;
	}

	
	float centerx = -p_x * sf;
	float centery = p_y * sf;
	position = glm::vec3(centerx, 0.0f, centery);
	modelType = type;
	rotateAngle = ra;
	ranslateDist = tdist;
}

Candel::~Candel() {
}
