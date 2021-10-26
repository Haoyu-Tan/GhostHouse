#ifndef TRACE_F_BULLET_INCLUDED
#define TRACE_F_BULLET_INCLUDED
#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "GlobalVar.h"
#include "MyStruct.h"
#include "Wall.h"
#include "StaticFurniture.h"
#include "WhiteGhost.h"
#include "BlueGhost.h"
#include "Door.h"
#include <vector>

class FBullet {
public:
	//var: position and transform
	glm::vec3 position;
	glm::vec3 newPosition;
	glm::mat4 modelMatrix;
	
	//glm::vec3 OriginFront;
	glm::vec3 direction;
	glm::vec3 motion;

	float sf; //scale factor

	int animeSI; // anime start index
	int totalVert;
	
	//var: animation
	//dModelType modelType; //what type of animation: MyStruct.h model type
	//std::vector<float>* dOM;
	float currentTime;
	float pry, prx;
	bool destroyed;

	FBullet(glm::vec3 pos, DModel* d, glm::vec3 dir,float ry, float rx);

	void Update(float dt, std::vector<Wall*> *w, std::vector<StaticFurniture*> *sfurn,
		std::vector<WhiteGhost*> *wgs, std::vector<BlueGhost*>* bgs, std::vector<Door*> *doors);
};





#endif // !TRACE_GHOST_INCLUDED

