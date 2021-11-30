#ifndef TRACE_UGLY_BALL_INCLUDED
#define TRACE_UGLY_BALL_INCLUDED
#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "GlobalVar.h"
#include "MyStruct.h"
#include "Wall.h"
#include "StaticFurniture.h"
#include <vector>

class UglyBall {
public:
	//var: position and transform
	glm::vec3 position;
	glm::mat4 modelMatrix;
	
	glm::vec3 OriginFront;
	glm::vec3 direction;

	glm::vec3 facing;

	float sf; //scale factor

	int action;
	int animeSI; // anime start index
	int totalVert;
	
	//var: animation
	//dModelType modelType; //what type of animation: MyStruct.h model type
	//std::vector<float>* dOM;
	float currentTime;

	bool destroyed;

	UglyBall(glm::vec3 pos, DModel* d);

	void Update(float dt, glm::vec3 pos, std::vector<Wall*> *w, std::vector<StaticFurniture*> *sfurn);
	void Generation(float dt, glm::vec3 pos);
	void ChangeFacing(glm::vec3 pdir);
	void Shot(float dt, std::vector<Wall*>* w, std::vector<StaticFurniture*>* sfurn);
	void CheckSwitch(float dt, glm::vec3 pos);
};





#endif // !TRACE_GHOST_INCLUDED

