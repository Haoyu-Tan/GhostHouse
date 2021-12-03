#ifndef FRUIT_INCLUDED
#define FRUIT_INCLUDED


#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/glm.hpp"


class Fruit {
public:
	int pos_x, pos_y;
	glm::vec3 color;

	glm::vec3 center;
	//glm::vec3 playerC;
	//float size;
	//float angle;
	float sf;

	//glm::mat4 modelm;
	float pickupdist;
	bool picked;

	//const float maxSpeed = 4.0;
	//float effectiveTime;

	Fruit();
	Fruit(int p_x, int p_y, float scaleFactor);
	~Fruit();

	//
	bool PickUp(glm::vec3 pos);
};

#endif
