#include "../include/Player.h"
#include <stdlib.h>
#include <ctype.h>


Player::Player(Scene* s) {
	if (s == nullptr) {
		printf("Scene could not be null!\n");
		exit(EXIT_FAILURE);
	}
	scene = s;
	position = scene->StartPosition();
	position.y = height;
	
	facing = glm::vec3(0.0f, 0.0f, 1.0f);
	direction = facing;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 front = facing + position;
	view = glm::lookAt(position, front, up);
	rotatex = 0.f;
	rotatey = 0.f;
	fw = false;
	bw = false;
	lft = false;
	rgt = false;
	rl = false;
	rr = false;
	ru = false;
	rd = false;
	mouse = false;
	jp = false;
	faint = false;
	attack = false;
	faintTime = 0.0;
	health = 100;
	torchStatus = 1;
	faintColor = glm::vec3(1.0, 1.0, 1.0);
	useGun = false;
	scene->pgun->renderS = useGun;
}

Player::~Player() {
}

void Player::fstart() {
	fw = true;
}

void Player::fend() {
	fw = false;
}

void Player::bstart() {
	bw = true;
}

void Player::bend() {
	bw = false;
}
void Player::lstart() {
	lft = true;
}

void Player::lend() {
	lft = false;
}
void Player::rstart() {
	rgt = true;
}

void Player::rend() {
	rgt = false;
}

void Player::rlstart() {
	rl = true;
}

void Player::rrstart() {
	rr = true;
}

void Player::rlend() {
	rl = false;
}
void Player::rrend() {
	rr = false;
}

void Player::rustart() {
	ru = true;
}

void Player::ruend() {
	ru = false;
}
void Player::rdstart() {
	rd = true;
}

void Player::rdend() {
	rd = false;
}

void Player::jump() {
	if (!jp) {
		jp = true;
		jumps = jumpstart;
	}
}

void Player::AttackStart() {
	attack = true;
}

void Player::AttackStop() {
	attack = false;
}

void Player::TorchONOFF() {
	if (torchStatus == 1) {
		torchStatus = 0;
		for (int i = 0; i < scene->whiteGhosts.size(); i++) {
			scene->whiteGhosts.at(i)->torchStatus = 0;
		}
		for (int i = 0; i < scene->blueGhosts.size(); i++) {
			scene->blueGhosts.at(i)->torchStatus = 0;
		}
		
	}
	else if (torchStatus == 0) {
		torchStatus = 1;
		for (int i = 0; i < scene->whiteGhosts.size(); i++) {
			scene->whiteGhosts.at(i)->torchStatus = 1;
		}
		for (int i = 0; i < scene->blueGhosts.size(); i++) {
			scene->blueGhosts.at(i)->torchStatus = 1;
		}
	}
}


void Player::rotate(int x, int y) {
	mouse = true;
	float y_lock = 75.f;
	rotatex -= ((float)x * sen);
	if (rotatex < 0) rotatex = rotatex + 360.f;
	if (rotatex > 360) rotatex = rotatex - 360.f;

	rotatey += ((float)y * sen);
	if (rotatey > y_lock) rotatey = y_lock;
	if (rotatey < -y_lock) rotatey = -y_lock;
}

void Player::HitByBall() {
	int bSize = scene->uglyBalls.size();
	for (int i = 0; i < bSize; i++) {
		UglyBall* ball = scene->uglyBalls.at(i);
		glm::vec3 ppdir = ball->position - position;
		ppdir.y = 0.0f;
		float dist = glm::length(ppdir);
		if (dist <= 0.3) {
			// hit by ball!!!
			faint = true;
			faintTime = 1;
			faintColor = glm::vec3(1.0, 0.0, 0.0);
			ball->destroyed = true;

			health -= 10;

			if (health <= 0) {
				scene->gameState = -1;
				health = 0;
			}
		}

	}
}

void Player::OpenDoor() {
	int bagSize = scene->pKeys.size();
	if (bagSize <= 0) return;
	int doorNum = scene->doors.size();
	for (int i = 0; i < doorNum; i++) {
		Door* door = scene->doors.at(i);
		glm::vec3 dirpd = position - door->position;
		dirpd.y = 0.0f;
		float dist = glm::length(dirpd);
		if (door->status == 2 && dist <= 2.0001) {
			delete scene->pKeys[bagSize - 1];
			scene->pKeys.erase(scene->pKeys.begin() + (bagSize - 1));
			door->status = 3;
			bagSize--;
			if (bagSize <= 0) break;
			// then we open door
			
		}
	}
}

void Player::Update(float dt) {
	// update faint
	if (faint) {
		CalculateFaintColor(dt);
		
	}
	HitByBall();
	CaughtByGhost();
	OpenDoor();
	PickUp();
	PickUpFruit();
	float dist = speed * dt;
	if (effectiveTime > 0) {
		dist = maxSpeed * dt;
		effectiveTime -= dt;
	}
	float turn = 100.f * dt;
	if (mouse) {
		rl = false;
		rr = false;
		ru = false;
		rd = false;
	}
	if (!(rl && rr)) {
		if (rl) {
			rotatex = rotatex + turn;

		}
		if (rr) {
			rotatex = rotatex - turn;
		}
		if (rotatex > 360) rotatex = rotatex - 360;
		if (rotatex < 0) rotatex = rotatex + 360;
	}

	if (!(ru && rd)) {
		if (ru) {
			rotatey = rotatey - turn;

		}
		if (rd) {
			rotatey = rotatey + turn;
		}
		if (rotatey > 75) rotatey = 75;
		if (rotatey < -75) rotatey = -75;
	}






	glm::mat4 rMat = glm::mat4(1);

	
	float rx = rotatex * 3.14f / 180;
	float ry = rotatey * 3.14f / 180;
	rMat = glm::rotate(rMat, rotatex * 3.14f / 180, glm::vec3(0.0f, 1.0f, 0.0f));
	rMat = glm::rotate(rMat, rotatey * 3.14f / 180, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec4 nfacing = glm::normalize(rMat * glm::vec4(facing, 1.0f));
	direction = glm::normalize(glm::vec3(nfacing.x, nfacing.y, nfacing.z));
	glm::vec4 nup = glm::normalize(rMat * glm::vec4(up, 1.0f));
	
	if (fw || bw || lft || rgt) {
		
		
		glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
		if (fw) {
			movement = movement + glm::normalize(glm::vec3(nfacing.x, 0.0f, nfacing.z));
		}
		if (bw) {
			movement = movement + glm::normalize(glm::vec3(-nfacing.x, 0.0f, -nfacing.z));
		}
		if (lft) {
			movement = movement + glm::normalize(glm::vec3(nfacing.z, 0.0f, -nfacing.x));
		}
		if (rgt) {
			movement = movement + glm::normalize(glm::vec3(-nfacing.z, 0.0f, nfacing.x));
		}
		
		if (movement.x != 0.0f || movement.z != 0.0f) {
			
			position = FinalMove(movement, dist);

			glm::vec3 goalPos = glm::vec3(-(scene->goal_x) * (scene->scaleFactor), position.y, (scene->goal_y) * (scene->scaleFactor));

			float distance = glm::length(goalPos - position);
			if (distance < scene->scaleFactor / 2) {
				printf("game win!!!!!!\n");
				scene->gameState = 1;
			}
		}
		
	}

	if (jp) {
		float njp = jumps - 10 * dt;
		float halfspeed = jumps - 5 * dt;
		float jd = halfspeed * dt;
		float ny = position.y + jd;
		if (ny <= height) {
			ny = height;
			jumps = 0.0;
			jp = false;
		}
		position = glm::vec3(position.x, ny, position.z);
		jumps = njp;

	}


	

	
	glm::vec3 front = glm::vec3(nfacing.x, nfacing.y, nfacing.z) + position;
	UpdateBag(glm::vec3(nfacing.x, nfacing.y, nfacing.z), nup, dt,ry,rx);

	view = glm::lookAt(position, front, glm::vec3(nup.x, nup.y, nup.z));
	mouse = false;

	(scene->healthBar)->update(position, glm::normalize(glm::vec3(nfacing.x, nfacing.y, nfacing.z)), nup, health,ry,rx,facing);

}


void Player::CaughtByGhost() {
	int wgSize = scene->whiteGhosts.size();
	for (int i = 0; i < wgSize; i++) {
		WhiteGhost* gost = scene->whiteGhosts.at(i);
		if (gost->CatchP(position)) {
			// caught by ghost

			health -= 35;
			if (health <= 0) {
				scene->gameState = -1;
				health = 0;
			}

			ghostNumber++;
			if (ghostNumber == 1) {
				speed = speed - 0.75;
			}
			else if (ghostNumber == 2) {
				speed = speed - 0.5;
				gost->hugAngle = -gost->hugAngle;
			}
			else if (ghostNumber == 3) {
				// lose
				scene->gameState = -1;
			}
			faint = true;
			faintTime = 1;
			faintColor = glm::vec3(1.0, 0.0, 0.0);
		}
	}
}

void Player::GunSwitch() {
	if (!useGun) {
		//we use gun now
		useGun = true;
		scene->pgun->renderS = true;
	}
	else {
		useGun = false;
		scene->pgun->renderS = false;
	}
}


void Player::UpdateBag(glm::vec3 nfacing, glm::vec3 nup, float dt, float ry, float rx) {
	
	int bagSize = scene->pKeys.size();
	glm::vec3 nright = glm::normalize(glm::cross(nfacing, nup));
	for (int i = bagSize - 1; i >= 0; i--) {
		Key* key = scene->pKeys.at(i);
		//glm::vec3 newCenter = position + glm::normalize(nup) * 0.035f - nright * 0.05f + nright * 0.0125f * (float)(i)+nfacing * 0.15f;
		//key->SetCenter(newCenter);
		//key->update(dt);
		key->UpdateByPlayer(position,nfacing,nup,nright,i,ry,rx,facing,dt);

	}
	(scene->pgun)->UpdateByPlayer(position, nfacing, nup, nright,ry, rx, facing, dt,attack,&scene->fbuls);
	
}

void Player::PickUp() {
	
	int keySize = (scene->keys).size();
	for (int i = keySize - 1; i >= 0; i--) {
		Key* key = scene->keys.at(i);
		if (key->PickUp(position)) {
			scene->pKeys.push_back(key);
			scene->keys.erase(scene->keys.begin() + i);
		}
	}
	int gunSize = (scene->guns).size();
	for (int i = gunSize - 1; i >= 0; i--) {
		Gun* g = scene->guns.at(i);
		if (g->PickUp(position)) {
			delete scene->guns[i];
			scene->guns.erase(scene->guns.begin() + i);
			scene->pgun->bulletNumber += 5;
		}
	}
	
}

void Player::PickUpFruit() {
	int fruitSize = (scene->fruits).size();
	for (int i = fruitSize - 1; i >= 0; i--) {
		Fruit* fruit = (scene->fruits).at(i);

		if (fruit->PickUp(position)) {
			if (fruit != nullptr) {
				delete fruit;
			}
			(scene->fruits).erase(scene->fruits.begin()+i);
			effectiveTime = 5.0;

		}
	}
}

void Player::CalculateFaintColor(float dt) {
	float ft = 1;
	faintTime = faintTime - dt;
	if (faintTime <= 0) {
		faintColor = glm::vec3(1.0, 1.0, 1.0);
		faint = false;
		return;
	}
	float weight = 1 - faintTime / ft;
	faintColor = glm::vec3(1.0,weight,  weight);
	
}


glm::vec3 Player::GetDirection() {
	return direction;
}

glm::vec3 Player::GetPosition() {
	return position;
}

glm::mat4 Player::GetView() {
	return view;
}


glm::vec3 Player::FinalMove(glm::vec3 move, float distance) {
	float sf = scene->scaleFactor;
	
	glm::vec3 result = move * distance;
	glm::vec3 pos = position + result;

	
	int oldx = -(int)round((position.x) / sf);
	int oldy = (int)round((position.z) / sf);
	float deltax = 0.0;
	float deltay = 0.0;
	if (move.x > 0) deltax = 0.2;
	else if (move.x < 0) deltax = -0.2;
	if (move.z > 0) deltay = 0.2;
	else if (move.z < 0) deltay = -0.2;

	int indexx = (int)round((pos.x + deltax) / sf);
	int indexy = (int)round((pos.z + deltay) / sf);
	
	if (indexx > 0) {
		result.x = 0.0;
		indexx = oldx;
	}
	if (indexy < 0) {
		result.z = 0.0;
		indexy = oldy;
	}
	
	indexx = -indexx;

	
	if (indexx >= scene->width) result.x = 0.0;
	if (indexy >= scene->height) result.z = 0.0;
	
	float left = fmin(pos.x, position.x);
	float right = fmax(pos.x, position.x);
	float top = fmax(pos.z, position.z);
	float bottom = fmin(pos.z, position.z);
	int wallSize = (scene->walls).size();
	bool hit = false;
	for (int i = 0; i < wallSize; i++) {
		Wall* wall = scene->walls.at(i);
		if (wall->CheckPosition(left, right, top, bottom)) {
			if (wall->pos_x != oldx && wall->pos_y != oldy) {
				
				if (wall->CheckPosition(position.x, position.x, top, bottom)) {
					result.z = 0.0;
				}
				if (wall->CheckPosition(left, right, position.z, position.z)) {
					
					result.x = 0.0;
				}
				hit = true;

			}
			else if (wall->pos_x != oldx) {
				result.x = 0.0;

			}
			else if (wall->pos_y != oldy) {
				result.z = 0.0;
			}

			pos = position + result;
			left = fmin(pos.x, position.x);
			right = fmax(pos.x, position.x);
			top = fmax(pos.z, position.z);
			bottom = fmin(pos.z, position.z);
		}
	}
	if(hit && result.z != 0.0 && result.x != 0.0) {
		result.x = 0.0;
		pos = position + result;
		left = fmin(pos.x, position.x);
		right = fmax(pos.x, position.x);
		top = fmax(pos.z, position.z);
		bottom = fmin(pos.z, position.z);
	}
	//// check static Furniture
	hit = false;
	for (int i = 0; i < scene->staticFurnitures.size(); i++) {
		StaticFurniture* sfurn = scene->staticFurnitures.at(i);
		if (sfurn->CheckPosition(left, right, top, bottom)) {
			if (sfurn->pos_x != oldx && sfurn->pos_y != oldy) {

				if (sfurn->CheckPosition(position.x, position.x, top, bottom)) {
					result.z = 0.0;
				}
				if (sfurn->CheckPosition(left, right, position.z, position.z)) {

					result.x = 0.0;
				}
				hit = true;

			}
			else if (sfurn->pos_x != oldx) {
				result.x = 0.0;

			}
			else if (sfurn->pos_y != oldy) {
				result.z = 0.0;
			}

			pos = position + result;
			left = fmin(pos.x, position.x);
			right = fmax(pos.x, position.x);
			top = fmax(pos.z, position.z);
			bottom = fmin(pos.z, position.z);
		}
	}
	if (hit && result.z != 0.0 && result.x != 0.0) {
		result.x = 0.0;
		pos = position + result;
		left = fmin(pos.x, position.x);
		right = fmax(pos.x, position.x);
		top = fmax(pos.z, position.z);
		bottom = fmin(pos.z, position.z);
	}





	hit = false;
	for (int i = 0; i < scene->doors.size(); i++) {
		Door* door = (scene->doors).at(i);

		if (door->currentHeight <= 3.0) {
			if (door->CheckPosition(left, right, top, bottom)) {
				if (door->pos_x != oldx && door->pos_y != oldy) {
					
					if (door->CheckPosition(position.x, position.x, top, bottom)) {
						result.z = 0.0;
					}
					if (door->CheckPosition(left, right, position.z, position.z)) {
						result.x = 0.0;
					}
					hit = true;
				}
				else if (door->pos_x != oldx) {
					result.x = 0.0;

				}
				else if (door->pos_y != oldy) {
					result.z = 0.0;
				}

				/*
				char dIndex = char(tolower((door->index)));
				int deleteKeyIndex = -1;
				
				for (int i = 0; i < bag.size(); i++) {
					Key* key = bag.at(i);
					if ((key->index) == dIndex) {
						door->isOpen = true;
						deleteKeyIndex = i;
						break;
					}
				}

				if (deleteKeyIndex != -1) {
					//delete key
					Key* deletedKey = bag.at(deleteKeyIndex);
					if (deletedKey != nullptr)
						delete deletedKey;
					bag.erase(bag.begin() + deleteKeyIndex);
				}
				*/
				
				pos = position + result;
				left = fmin(pos.x, position.x);
				right = fmax(pos.x, position.x);
				top = fmax(pos.z, position.z);
				bottom = fmin(pos.z, position.z);
				
			}
		}
		
	}
	if (hit && result.z != 0.0 && result.x != 0.0) {
		result.x = 0.0;
		pos = position + result;
		left = fmin(pos.x, position.x);
		right = fmax(pos.x, position.x);
		top = fmax(pos.z, position.z);
		bottom = fmin(pos.z, position.z);
	}

	return (position + result);
}