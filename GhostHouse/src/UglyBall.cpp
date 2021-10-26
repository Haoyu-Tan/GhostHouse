#include "../include/UglyBall.h"

UglyBall::UglyBall(glm::vec3 pos, DModel* d) {
	position = glm::vec3(pos.x,1.8, pos.z);
	animeSI = d->animations.at(2)->frames.at(0)->startIndex;
	totalVert = d->animations.at(2)->frames.at(0)->totalVert;
	action = 0;
	sf = 0.0; // max 2
	currentTime = 0.0f;
	destroyed = false;
	facing = glm::vec3(0.0, 0.0, 0.0);
}


// Always facing to the player
void UglyBall::ChangeFacing(glm::vec3 pdir) {
	glm::vec3 front = glm::vec3(OriginFront.x, 0.0, OriginFront.z);
	front = glm::normalize(front);
	float dp = pdir.x * front.x + pdir.z * front.z;
	float det = pdir.x * front.z - pdir.z * front.x;
	facing.y = atan2(det, dp);
	//printf("facing is %f\n",facing.y);

}


void UglyBall::CheckSwitch(float dt, glm::vec3 pos) {
	currentTime = currentTime + dt;
	if (currentTime >= 1.75) {
		action = 1;
		currentTime = 1.5f;
		sf = 2.0f;
		glm::vec3 dir = pos - position;
		direction = glm::normalize(glm::vec3(dir.x, 0.0f, dir.z));
	}
}

void UglyBall::Generation(float dt,glm::vec3 pos) {
	glm::vec3 dir = pos - position;
	ChangeFacing(dir);
	sf = currentTime / 1.75 * 2.0;
	facing.x = facing.x + dt * 3.14;
	//if (facing.x >= 3.1415926) facing.x -= 3.1415926;
}


void UglyBall::Shot(float dt, std::vector<Wall*>* w, std::vector<StaticFurniture*>* sfurn) {
	glm::vec3 newPosition = position + dt * 8.0f * direction;
	// check hit is in player
	// here we only need to check self destroy
	
	// Check whether self destory
	currentTime = currentTime - dt;
	if (currentTime <= 0) {
		destroyed = true;
		return;
	}

	float left = fmin(newPosition.x, position.x);
	float right = fmax(newPosition.x, position.x);
	float top = fmax(newPosition.z, position.z);
	float bottom = fmin(newPosition.z, position.z);
	// check collision
	for (int i = 0; i < sfurn->size(); i++) {
		StaticFurniture* sfn = sfurn->at(i);
		if (sfn->CheckObjPosition(left, right, top, bottom)) {
			destroyed = true;
			return;
		}
	}
	for (int i = 0; i < w->size(); i++) {
		Wall* wall = w->at(i);
		if (wall->CheckPosition(left, right, top, bottom)) {
			destroyed = true;
			return;
		}
	}


	// ub update position and angle
	facing.x = facing.x + dt * 3.1415926;
	//if (facing.x >= 3.1415926) facing.x -= 3.1415926;
	position = newPosition;
}

void UglyBall::Update(float dt, glm::vec3 pos, std::vector<Wall*>* w, std::vector<StaticFurniture*>* sfurn) {
	// change action
	if (destroyed) return;
	if (action == 0) CheckSwitch(dt,pos);

	switch (action) {
	case 0:
		Generation(dt, pos);
		// generate
		break;
	case 1:
		Shot(dt,w,sfurn);
		// shot!
		break;
	}

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(sf, sf, sf));
	modelMatrix = glm::rotate(modelMatrix, facing.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, facing.x, glm::vec3(1.0f, 0.0f, 0.0f));
	
}