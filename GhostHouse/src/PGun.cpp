#include "../include/PGun.h"

PGun::PGun(DModel* d) {
	dm = d;
	modelMatrix = glm::mat4(1);
	bulletNumber = 5;
	renderS = true;
	animeSI = d->animations.at(0)->frames.at(0)->startIndex;
	totalVert = d->animations.at(0)->frames.at(0)->totalVert;
	size = 0.5;
	reloadTime = 0.0;
	action = 0;
	angleX = 0.0;

	banimeSI = d->animations.at(1)->frames.at(0)->startIndex;
	btotalVert = totalVert = d->animations.at(0)->frames.at(0)->totalVert;
	bangle = 0;
	bsize = 0.05f;
}


void PGun::UseGunOrNot() {
	renderS = !renderS;
}


void PGun::Shot(glm::vec3 pos, glm::vec3 fdir, std::vector<FBullet*>* fbuls,float ry, float rx) {
	bulletNumber--;
    // generate new bullet
	FBullet* fb = new FBullet(pos,dm,fdir,ry,rx);
	fbuls->push_back(fb);
	reloadTime = 0.8;
	action = 1;
}


void PGun::Reload(float dt) {
	reloadTime = reloadTime - dt;
	if (reloadTime <= 0) {
		reloadTime = 0.0;
		angleX = 0.0;
		action = 0;
	}
	else if (reloadTime >= 0.6) {
		angleX = 30.0f * (0.2f - (reloadTime - 0.6f)) / 0.2f;
	}
	else if (reloadTime >= 0.2) {
		angleX = 30.0f * (reloadTime-0.2f) / 0.4f;
	}
}


void PGun::UpdateByPlayer(glm::vec3 ppos, glm::vec3 pf, glm::vec3 pup, glm::vec3 nright, 
	 float ry, float rx, glm::vec3 facing, float dt, bool attack, std::vector<FBullet*>* fbuls) {
	bool canShot = true;
	float Pi = 3.14159265358979323846;

	if (bulletNumber <= 0) {
		canShot = false;
	}
	if (action == 0) {
		// ready to shot
		if (canShot && attack && renderS) {
			//float Pi = 3.14159265358979323846;
			//glm::vec4 newPF = glm::rotate(glm::mat4(1), -(10.0f) * Pi / 180, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(pf.x,pf.y,pf.z,1.0);
			Shot(ppos, facing,fbuls,ry,rx);
		}
	}
	else if (action == 1) {
		// reload
		Reload(dt);


	}
	if (!renderS) return;
	// Otherwise update
	// go with player
	glm::vec3 center = ppos - pup * 0.065f;
	glm::vec3 pos1 = facing * 0.2f;

	glm::vec3 rotateAngle = glm::vec3(0.0, 0.0, 0.0);
	rotateAngle.y = rx;
	rotateAngle.x = ry;

	//float angleS = 90;

	modelMatrix = glm::mat4(1);
	
	modelMatrix = glm::translate(modelMatrix, center);
	modelMatrix = glm::rotate(modelMatrix, rotateAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotateAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, pos1);

	modelMatrix = glm::rotate(modelMatrix, -(5.0f + angleX) * Pi / 180, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, -0.5f * Pi,glm::vec3(0.0f,1.0f,0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size, size, size));
	

	// next we need to update bullet UI
	bangle = bangle + dt * 360.0f;
	if (bangle >= 360.0f) {
		bangle = bangle - 360.0f;
	}

	center = ppos - pup * 0.075f - nright * 0.07f;
	pright = nright;
	pos1 = facing * 0.15f;
	bmmatrix = glm::translate(glm::mat4(1), center);
	bmmatrix = glm::rotate(bmmatrix, rotateAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	bmmatrix = glm::rotate(bmmatrix, rotateAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));
	bmmatrix = glm::translate(bmmatrix, pos1);
	bmmatrix = glm::rotate(bmmatrix, bangle * Pi / 180, glm::vec3(0.0f, 1.0f, 1.0f));
	bmmatrix = glm::scale(bmmatrix, glm::vec3(bsize, bsize, bsize));
}

glm::mat4 PGun::GetBMatrix(int index) {
	glm::mat4 result = glm::translate(glm::mat4(1), float(index) * 0.005f * pright);
	result = result * bmmatrix;
	return result;
}

PGun::~PGun() {
}