#ifndef GUN_INCLUDED
#define GUN_INCLUDED

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/glm.hpp"
#include "MyStruct.h"

class Gun {
public:
	glm::vec3 position;
	int animeSI; // anime start index
	int totalVert;
	float angle;
	float sf;
	float size;
	glm::mat4 modelm;
	Gun(int x, int y, float scaleFactor, DModel* d);
	~Gun();
	void Update(float dt);
	bool PickUp(glm::vec3 pos);
};



#endif
