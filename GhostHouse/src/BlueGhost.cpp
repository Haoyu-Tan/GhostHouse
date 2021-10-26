#include "../include/BlueGhost.h"

BlueGhost::BlueGhost(glm::vec3 pos, DModel* d) {
	position = glm::vec3(pos.x,1.3f,pos.z);
	
	sf = 1.25;

	OriginFront = glm::vec3(0.0f, 0.0f, 1.0f);
	facing = glm::vec3(0.0f, 0.0f, 0.0f);// angle
	PatrolDir = glm::vec3(0.0f, 0.0f, 0.0f);

	



	dm = d;

	updown = 0.2;
	y = 1.3;
	
	animationStatus = 0;
	//dOM = f;
	updateTime = 0.02f;
	currentTime = 0.0f;
	aIndex = 0;
	totalVert = dm->animations.at(animationStatus)->frames.at(aIndex)->totalVert;
	animeSI = dm->animations.at(animationStatus)->frames.at(aIndex)->startIndex;
	//printf("in d size is %i\n", dm->animations.size());
	//printf("in dom size is %i\n", dOM->size());
	freezeTime = 0.0;
	action = 0;
	turnTime = 0.0;
	
	tIndex = 0;
	torchStatus = 1;
	shotcd = 0.0;
}


void BlueGhost::Animate(float dt) {

	if (shotcd > 0) return;

	//myVert.clear();

	Animation* anim = dm->animations.at(animationStatus);
	// Next frame?
	float fac = 1;
	if (action == 0) {
		aIndex = 0;
		totalVert = anim->frames.at(aIndex)->totalVert;
		animeSI = anim->frames.at(aIndex)->startIndex;
		return;
	}
	currentTime = currentTime + dt;
	if (animationStatus == 0 && aIndex == anim->frames.size() - 1) {
		fac = 26;
	}
	if (currentTime >= updateTime * fac) {
		// change frame
		
		int num = floor(currentTime / updateTime/fac);
		currentTime = currentTime - num * updateTime*fac;
		aIndex = aIndex + num;
		
		float size = anim->frames.size();
		
		if (aIndex >= size) {
			if (animationStatus == 0) {
				animationStatus = 1; // continue to shot
				//updateTime = 0.02;
				currentTime = 0.0f;
				aIndex = 0;
				anim = dm->animations.at(animationStatus);

			}
			else if(animationStatus == 1){
				//currentTime = 0.0f;
				aIndex = 0;
				animationStatus = 0;
				currentTime = 0.0f;
				anim = dm->animations.at(animationStatus);
				ball = false;
				shotcd = 0.5f;
				ub = nullptr;
				//ballId = -1; //no more control on bullet
			}
		}
		totalVert = anim->frames.at(aIndex)->totalVert;
		animeSI = anim->frames.at(aIndex)->startIndex;
	}
	
}




// Always facing to the player
void BlueGhost::ChangeFacing(glm::vec3 pdir) {
	glm::vec3 front = glm::vec3(OriginFront.x, 0.0, OriginFront.z);
	front = glm::normalize(front);
	float dp = pdir.x * front.x + pdir.z * front.z;
	float det = pdir.x * front.z - pdir.z * front.x;
	facing.y = atan2(det, dp);
	//printf("facing is %f\n",facing.y);

}

void BlueGhost::UpDown(float dt) {
	//if (action == -1) return;

	position.y = position.y + updown * dt;
	if (position.y >= y + 0.1) {
		position.y = y + 0.1;
		updown = -updown;
	}
	if (position.y <= y - 0.1) {
		position.y = y - 0.1;
		updown = -updown;
	}

}


bool BlueGhost::CheckHit(glm::vec3 bpos) {
	float hitdist = 0.2f;
	glm::vec3 dir = bpos - position;
	float distance = glm::length(dir);
	if (distance <= hitdist) {
		freezeTime = 5.0f;
		if (action == 1) {
			if (ub != nullptr) {
				if (ub->action == 0) {
					ub->destroyed = true;
					ub = nullptr;
				}
			}
		}
		
		return true;
	}
	return false;
}


void BlueGhost::Shot(float dt, glm::vec3 pp, std::vector<UglyBall*> *uglyb) {
	glm::vec3 dir = glm::normalize(pp - position);
	ChangeFacing(dir);
	if (shotcd > 0) {
		shotcd -= dt;
		// check whether we need next shot

		return;
	}
	if (!ball) {
		ub = new UglyBall(position, dm);
		uglyb->push_back(ub);
		ball = true;
	}
	

	// generate bullet
	
}

void BlueGhost::Patrol(float dt) {
	// reset time
	turnTime = turnTime - dt;
	if (turnTime <= 0.0) {
		turnTime = 1.0;
		tIndex ++;
		if (tIndex == 4) {
			tIndex = 0;
		}
	}
	float turnAngle = 0.5 * (float)tIndex * 3.1415926;
	facing.y = turnAngle;
	glm::vec4 nf = glm::rotate(glm::mat4(1), turnAngle, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(OriginFront.x, OriginFront.y, OriginFront.z, 1.0f);
	PatrolDir = glm::normalize(glm::vec3(nf.x, nf.y, nf.z));
	UpDown(dt);
}

void BlueGhost::FakeAI(glm::vec3 pp) {
	float detectDistance = 8.0;
	if (torchStatus == 0) detectDistance = 0.75 * detectDistance;
	glm::vec3 dir = pp - position;
	dir.y = 0.0f;
	float distance = glm::length(dir);
	if (action == 0) {
		if (distance <= detectDistance) {
			float dpBound = 0.4;
			dir = glm::normalize(dir);
			float dp = glm::dot(dir, PatrolDir);
			if (dp>=dpBound) {
				//Start shotting
				action = 1;
				// Remember to set Patrol Type
				turnTime = 1.0;
			}
		}
	}
	else if (action == 1 && !ball && distance > detectDistance) {
		// we get the cloest angle
		
		action = 0;
		glm::vec3 front = glm::vec3(OriginFront.x, 0.0, OriginFront.z);
		front = glm::normalize(front);
		glm::vec3 pdir = glm::normalize(glm::vec3(dir.x, 0.0, dir.z));
		float dp = pdir.x * front.x + pdir.z * front.z;
		float det = pdir.x * front.z - pdir.z * front.x;
		float angle = atan2(det, dp);
		tIndex = (int)round(angle/3.1415926*2.0);
		animationStatus = 0;
		aIndex = 0;
	}
}


void BlueGhost::Update(float dt,glm::vec3 pp,glm::vec3 pd, std::vector<UglyBall*> *uglyb) {
	// AI
	
	if (freezeTime > 0) {
		freezeTime -= dt;
		return;
	}
	FakeAI(pp);



	switch (action) {

	case 0: // Patrol
		//printf("I am patrolling");
		Patrol(dt);
		break;
	case 1: // Shooting
		Shot(dt, pp, uglyb);
		break;
	case -1:
		//Hug(pp,pd);
		break;
	}
	


	// Update Matrix
	
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(sf, sf, sf));
	modelMatrix = glm::rotate(modelMatrix, facing.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, facing.x, glm::vec3(1.0f, 0.0f, 0.0f));
	// animation
	Animate(dt);


	// 
}

