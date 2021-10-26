#ifndef STATIC_FURNITURE_INCLUDED
#define STATIC_FURNITURE_INCLUDED

#include "GlobalVar.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

class StaticFurniture {
public:
	float pos_x, pos_y;
	float left, right, top, bottom;
	float left1, right1, top1, bottom1;
	sModelType modelType;
	glm::vec3 rotateAngle;
	glm::vec3 ranslateDist;

	StaticFurniture();
	StaticFurniture::StaticFurniture(float p_x, float p_y, float sf, sModelType type, glm::vec3 ra, glm::vec3 tdist,int gtype);
	~StaticFurniture();
	bool CheckPosition(float l, float r, float t, float b);
	bool CheckObjPosition(float l, float r, float t, float b);
};

#endif // !1

