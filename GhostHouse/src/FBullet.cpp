#include "../include/FBullet.h"

FBullet::FBullet(glm::vec3 pos, DModel* d,glm::vec3 dir,float ry, float rx) {
	//OriginFront = glm::vec3(0.0f, 0.0f, -1.0f);
	position = pos;
	newPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = dir;
	pry = ry;
	prx = rx;
	motion = direction * 0.5f;
	//printf("FBULLET ORIGINAL POSITION IS %f %f %f\n", pos.x, pos.y, pos.z);
	//printf("FBULLET ORIGINAL DIRECTION IS %f %f %f\n", dir.x, dir.y, dir.z);
	animeSI = d->animations.at(1)->frames.at(0)->startIndex;
	totalVert = d->animations.at(1)->frames.at(0)->totalVert;
	sf = 1.2; // max 2
	currentTime = 0.0f;
	destroyed = false;
	//facing = glm::vec3(0.0, 0.0, 0.0);

}


void FBullet::Update(float dt, std::vector<Wall*>* w, std::vector<StaticFurniture*>* sfurn,
	std::vector<WhiteGhost*>* wgs, std::vector<BlueGhost*>* bgs, std::vector<Door*>* doors) {
	float speed = 8.0f;
	// then we check whether the bullet is hitting a ghost


	currentTime += dt;
	if (currentTime >= 2.0f) {
		destroyed = true;
		return;
	}


	//glm::vec3 newPos = position + speed *dt* direction;
	//
	// check collision
	motion += direction * speed * dt;
	glm::mat4 mm = glm::rotate(glm::mat4(1), prx, glm::vec3(0.0f, 1.0f, 0.0f));
	mm = glm::rotate(mm, pry, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec4 newPos4 = mm * glm::vec4(motion.x, motion.y, motion.z, 1.0);
	glm::vec3 newPos = glm::vec3(newPos4.x, newPos4.y, newPos4.z);
	newPos += position;
	//printf("FBULLET POSITION IS %f %f %f\n", newPos.x, newPos.y, newPos.z);
	

	//
	newPosition = newPos;

	float left = fmin(newPosition.x, newPos.x);
	float right = fmax(newPosition.x, newPos.x);
	float top = fmax(newPosition.z, newPos.z);
	float bottom = fmin(newPosition.z, newPos.z);

	/*
	for (int i = 0; i < sfurn->size(); i++) {
		StaticFurniture* sfn = sfurn->at(i);
		if (sfn->CheckObjPosition(left, right, top, bottom)) {
			destroyed = true;
			return;
		}
	}
	*/
	for (int i = 0; i < w->size(); i++) {
		Wall* wall = w->at(i);
		if (wall->CheckPosition(left, right, top, bottom)) {
			destroyed = true;
			return;
		}
	}
	/*
	for (int i = 0; i < doors->size(); i++) {
		Door* door = doors->at(i);
		if (door->CheckPosition(left, right, top, bottom)) {
			if (door->currentHeight < newPosition.y) {
				destroyed = true;
				return;
			}
			
		}
	}
	*/

	for (int i = 0; i < bgs->size(); i++) {
		BlueGhost* bg = bgs->at(i);
		if (bg->CheckHit(newPos)) {
			destroyed = true;
			return;
		}
	}
	for (int i = 0; i < wgs->size(); i++) {
		WhiteGhost* wg = wgs->at(i);
		if (wg->CheckHit(newPos)) {
			destroyed = true;
			return;
		}
	}
    

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, prx, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, pry, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, motion);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(sf, sf, sf));



}
