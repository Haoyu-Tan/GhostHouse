#include "../include/WhiteGhost.h"

WhiteGhost::WhiteGhost(glm::vec3 pos, DModel* d) {
	position = glm::vec3(pos.x,1.5f,pos.z);
	
	sf = 1.5;

	y = 1.5;
	updown = 0.2;
	OriginFront = glm::vec3(0.0f, 0.0f, 1.0f);
	facing = glm::vec3(0.0f, 0.0f, 0.0f);
	dm = d;
	torchStatus = 1;
	animationStatus = 0;
	//dOM = f;
	updateTime = 0.02f;
	currentTime = 0.0f;
	aIndex = 0;
	totalVert = dm->animations.at(animationStatus)->frames.at(aIndex)->totalVert;
	animeSI = dm->animations.at(animationStatus)->frames.at(aIndex)->startIndex;
	//printf("in d size is %i\n", dm->animations.size());
	//printf("in dom size is %i\n", dOM->size());
	speed = 1.5;
	action = 0;
	PatrolType = -1;
	PatrolDir = glm::vec3(0.0f, 0.0f, 0.0f);
	PatrolMid = position;
	hugAngle = 3.1415926 * 45.0f / 180.0f;
	freezeTime = 0.0f;
}

void WhiteGhost::Animate(float dt) {
	int OneVertSize = 22;


	if (animationStatus < 0) {
		return;
	}

	//myVert.clear();

	Animation* anim = dm->animations.at(animationStatus);

	// Next frame?
	float fac = 1;
	if (action == 1) {
		fac = 0.25;
	}
	currentTime = currentTime + dt;
	if (currentTime >= updateTime * fac) {
		// change frame
		
		int num = floor(currentTime / updateTime/fac);
		currentTime = currentTime - num * updateTime*fac;
		aIndex = aIndex + num;
		
		float size = anim->frames.size();
		if (aIndex >= size) {
			aIndex = floor(aIndex/size);
		}
		totalVert = anim->frames.at(aIndex)->totalVert;
		animeSI = anim->frames.at(aIndex)->startIndex;
	}
	
	// interpolation
	//float w1 = currentTime / updateTime;
	//printf("Weight 1 is %f\n", w1);
	//float w0 = 1.0f - w1;
	//float nextIndex = aIndex + 1;
	//if (nextIndex >= anim->frames.size()) {
	//	nextIndex = 0;
	//}

	// get the frame
	


	//int f0index = anim->frames.at(aIndex)->startIndex * 22;
	//int f1index = anim->frames.at(nextIndex)->startIndex *22;
	//printf("f0 index is %i \n", f0index);
	//printf("f1 index is %i \n", f1inde x);
	//printf("in dom size is %i\n", dOM->size());

	// do interpolation
	/*
	for (int i = 0; i < totalVert; i++) {
		for (int j = 0; j < OneVertSize; j++) {
			
			if (j > 2 && j < 19) {
				myVert.push_back(dOM->at(f0index));
				//if (aIndex >= 1) {
					//printf("PUSH BACK %f\n", dOM->at(f1index));
				//}
				
			}
			else {
				//myVert.push_back(dOM->at(f1index));
				//if (aIndex >= 1) {
					//printf("PUSH BACK %f\n", dOM->at(f1index));
				//}
				/*
				if (w0 <= 0.25){
					w0 = 0.25;
					w1 = 0.75;
				}
				else if (w0 > 0.25 && w0 <= 0.5) {
					w0 = 0.5;
					w1 = 0.5;

				}
				else if (w0 < 0.5 && w0 >= 0.75) {
					w0 = 0.75;
					w1 = 0.25;
				}
				else {
					w0 = 1.0;
					w1 = 0.0;
				}
				myVert.push_back(dOM->at(f0index) * w0 + dOM->at(f1index) * w1);
				//myVert.push_back(dOM->at(f0index) * 1 + dOM->at(f1index) * 0);
			}
			//printf("f0 index is %i \n", f0index);
			//printf("f1 index is %i \n", f1index);
			//printf("DOM total is %i \n",dOM->size());
			f0index++;
			f1index++;
		}
		
		
	}
	*/
	//printf("f0 index is: %d, f1 index is: %d\n", f0index, f1index);
	//printf("size of myVert is %i\n", myVert.size());
	//for (int i = 0; i < myVert.size(); i++) {
	//	printf("value is %f ", myVert.at(i));
	//}
}


bool WhiteGhost::CheckHit(glm::vec3 bpos) {
	float hitdist = 0.25f;
	glm::vec3 dir = bpos - position;
	float distance = glm::length(dir);
	if (distance <= hitdist && action >= 0) {
		freezeTime = 5.0f;
		return true;
	}
	return false;
}


// Always facing to the player
void WhiteGhost::ChangeFacing(glm::vec3 pdir) {
	glm::vec3 front = glm::vec3(OriginFront.x, 0.0, OriginFront.z);
	front = glm::normalize(front);
	float dp = pdir.x * front.x + pdir.z * front.z;
	float det = pdir.x * front.z - pdir.z * front.x;
	facing.y = atan2(det, dp);
	//printf("facing is %f\n",facing.y);

}

void WhiteGhost::Hug(glm::vec3 pp, glm::vec3 pd) {
	float distFactor = 0.2;

	glm::vec3 dir = pp - position;
	dir.y = 0.0f;
	dir = glm::normalize(dir);
	ChangeFacing(dir);
	glm::vec3 ghostdir = glm::vec3(pd.x, 0.0f, pd.z);
	ghostdir = glm::normalize(ghostdir);
	ghostdir = distFactor * ghostdir;
	glm::vec4 temp = glm::vec4(ghostdir.x, ghostdir.y, ghostdir.z,1.0);
	glm::mat4 rotation = glm::rotate(glm::mat4(1), hugAngle, glm::vec3(0.0, 1.0, 0.0));
	temp = rotation * temp;
	position = glm::vec3(temp.x + pp.x, pp.y-0.6f, temp.z + pp.z);
}


void WhiteGhost::UpDown(float dt) {
	if (action == -1) return;
	
	position.y = position.y + updown * dt;
	if (position.y >= y + 0.1) {
		position.y = y + 0.1;
		updown = -updown;
	}
	if(position.y <= y - 0.1){
		position.y = y - 0.1;
		updown = -updown;
	}
	
}




void WhiteGhost::Chasing(float dt,glm::vec3 pp) {
	// get direction
	glm::vec3 dir = pp - position;
	dir.y = 0.0f;
	dir = glm::normalize(dir);
	ChangeFacing(dir); // Actually change facing to a given point
	// head down a little bit
	facing.x = 1;
	position = position + dir * speed * dt;
}

void WhiteGhost::Patrol(float dt) {
	if (PatrolType == -1) {
		PatrolMid = position;
		PatrolType = rand() % 2;
		facing.x = 0.5;
		if (PatrolType == 0) PatrolDir = glm::vec3(1.0f, 0.0f, 0.0f);
		else PatrolDir = glm::vec3(0.0f, 0.0f, -1.0f);
		ChangeFacing(PatrolDir);
	}
	float speedFactor = 0.8;
	float boundary = 2;
	if (PatrolType == 0) {
		position.x = position.x + speedFactor * speed * dt * PatrolDir.x; 
		float dist = position.x - PatrolMid.x;

		if (dist >= boundary) {
			position.x = PatrolMid.x + boundary;
			PatrolDir.x = -1 * PatrolDir.x;
			ChangeFacing(PatrolDir);
		}
		if (dist <= -boundary) {
			position.x = PatrolMid.x - boundary;
			PatrolDir.x = -1 * PatrolDir.x;
			ChangeFacing(PatrolDir);
		}

	}
	else {
		position.z = position.z +  speedFactor * speed * dt * PatrolDir.z;
		float dist = position.z - PatrolMid.z;
		if (dist >= boundary) {
			position.z = PatrolMid.z + boundary;
			PatrolDir.z = -1*PatrolDir.z;
			ChangeFacing(PatrolDir);
		}
		if (dist <= -boundary) {
			position.z = PatrolMid.z - boundary;
			PatrolDir.z = -1 * PatrolDir.z;
			ChangeFacing(PatrolDir);
		}
	}
	

}


void WhiteGhost::FakeAI(glm::vec3 pp) {
	if (action == -1) return;
	float DetectDistance = 6.0f;
	if (torchStatus == 0) DetectDistance = 0.75 * DetectDistance;
	glm::vec3 dir = pp - position;
	dir.y = 0.0f;
	float distance = glm::length(dir);
	if (action == 0 && distance <= DetectDistance) {
		// further check;
		float dpBound = 0.4;
		dir = glm::normalize(dir);
		float dp = glm::dot(dir, PatrolDir);
		if (dpBound >= 0.4) {
			//Start chasing
			action = 1;
			// Remember to set Patrol Type
			PatrolType = -1;
		}
	}
	if (action == 1 && distance > DetectDistance) {
		action = 0;
	}
}


bool WhiteGhost::CatchP(glm::vec3 pp) {
	if (action != 1) return false;
	if (freezeTime > 0.0) return false;
	float cdist = 1;
	glm::vec3 dir = pp - position;
	dir.y = 0.0f;
	if (glm::length(dir) <= cdist) {
		// change animation
		y = pp.y - 0.6f;
		position.y = y;
		action = -1;
		facing.x = 0.0;
		sf = 0.6;
		animationStatus = 1;
		aIndex = 0;
		currentTime = 0.0;
		updateTime = 0.05f;
		return true;
		

		
	}
	return false;
}


void WhiteGhost::Update(float dt,glm::vec3 pp,glm::vec3 pd) {
	// AI
	if (freezeTime > 0) {
		freezeTime -= dt;
		return;
	}
	UpDown(dt);
	FakeAI(pp);



	switch (action) {

	case 0: // Patrol
		//printf("I am patrolling");
		Patrol(dt);
		break;
	case 1: // Chasing
		Chasing(dt, pp);
		break;
	case -1:
		Hug(pp,pd);
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

