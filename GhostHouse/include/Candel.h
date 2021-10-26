#ifndef CANDEL_INCLUDED
#define CANDEL_INCLUDED

#include "GlobalVar.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

class Candel {
public:
	//float pos_x, pos_y;
	glm::vec3 position;
	float left, right, top, bottom;
	float left1, right1, top1, bottom1;
	sModelType modelType;
	glm::vec3 rotateAngle;
	glm::vec3 ranslateDist;

	Candel(float p_x, float p_y, float sf, sModelType type, glm::vec3 ra, glm::vec3 tdist, int gtype);
	~Candel();
};

#endif // !1

