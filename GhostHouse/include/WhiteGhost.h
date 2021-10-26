#ifndef TRACE_GHOST_INCLUDED
#define TRACE_GHOST_INCLUDED
#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "GlobalVar.h"
#include "../include/MyStruct.h"
#include <vector>

class WhiteGhost {
public:
	//var: position and transform
	glm::vec3 position;
	glm::mat4 modelMatrix;
	glm::vec3 OriginFront;
	int torchStatus;
	float sf; //scale factor

	glm::vec3 facing;
	float y;
	float updown;
	float speed; // chasing speed
	int action;
	// action 0 is patrol
	glm::vec3 PatrolMid;
	glm::vec3 PatrolDir;
	int PatrolType;
	// 0 horizontal
	// 1 vertical
	float freezeTime;
	// action 1 is chasing

	float hugAngle;
	
	//var: animation
	//dModelType modelType; //what type of animation: MyStruct.h model type
	DModel* dm;
	//std::vector<float>* dOM;


	// Following is for animation
	int animationStatus; //index: MyStruct.h which animation
	int aIndex; //# of model in an animation
	
	int animeSI; // anime start index
	int totalVert;

	float updateTime; //amount of time to update
	float currentTime; //currentTime += deltaTime

	WhiteGhost(glm::vec3 pos, DModel* d);
	void Animate(float dt);
	void Update(float dt,glm::vec3 pp, glm::vec3 pd);
	void ChangeFacing(glm::vec3 pdir);
	void UpDown(float dt);
	void Chasing(float dt, glm::vec3 pp);
	void Patrol(float dt);
	void Hug(glm::vec3 pp,glm::vec3 pd);
	void FakeAI(glm::vec3 pp);
	bool CatchP(glm::vec3 pp); // before this update
	bool CheckHit(glm::vec3 bpos);
};

#endif // !TRACE_GHOST_INCLUDED

