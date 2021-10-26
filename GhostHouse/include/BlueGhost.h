#ifndef TRACE_GHOST_B_INCLUDED
#define TRACE_GHOST_B_INCLUDED
#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "GlobalVar.h"
#include "MyStruct.h"
#include "UglyBall.h"
#include <vector>

class BlueGhost {
public:
	//var: position and transform
	glm::vec3 position;
	//float left, right, top, bottom, high, low;
	glm::mat4 modelMatrix;
	glm::vec3 OriginFront;
	int torchStatus;
	float sf; //scale factor
	float y;
	float updown;

	glm::vec3 facing;
	int action;
	// action 0 is updown
	// After it detected player action 1 is shoot
	float turnTime;
	int tIndex;
	
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
	float shotcd;
	glm::vec3 PatrolDir;
	
	bool ball = false;
	UglyBall* ub;
	float freezeTime = 0.0;


	BlueGhost(glm::vec3 pos, DModel* d);
	void FakeAI(glm::vec3 pp);
	void UpDown(float dt);
	
	void Animate(float dt); 
	void Update(float dt,glm::vec3 pp, glm::vec3 pd, std::vector<UglyBall*> *uglyb);
	void ChangeFacing(glm::vec3 pdir);
	//void FakeAI(glm::vec3 pp);
	void Patrol(float dt);
	void Shot(float dt,glm::vec3 pp, std::vector<UglyBall*> *uglyb);
	bool CheckHit(glm::vec3 bpos);
};





#endif // !TRACE_GHOST_INCLUDED

