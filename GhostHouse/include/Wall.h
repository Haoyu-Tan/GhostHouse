#ifndef WALL_INCLUDED
#define WALL_INCLUDED
#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

class Wall {
public:
	int pos_x, pos_y;
	float left, right, top, bottom;
	glm::vec3 rotateAngle;
	glm::vec3 scale;
	glm::vec3 position;
	Wall();
	Wall(int p_x, int p_y, float sf, glm::vec3 ra,glm::vec3 s);
	bool CheckPosition(float l, float r, float t, float b);
};

#endif
