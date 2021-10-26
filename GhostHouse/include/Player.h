#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "Scene.h"

class Player {
public:
	glm::vec3 position;
	glm::vec3 facing;
	glm::vec3 up;
	glm::mat4 view;
	glm::vec3 direction;
	float height = 1.8;
	float sen = 0.25; // control mouse
	float speed = 3;
	float maxSpeed = 5.0;
	int ghostNumber = 0;
	bool useGun;

	float effectiveTime = -1.0;
	bool fw, bw, lft, rgt, rl, rr, ru, rd, mouse, jp;
	float jumps; // jump speed
	float jumpstart = 5;
	float rotatex, rotatey;
	float rx, ry;
	float torchStatus;
	bool attack;
	Scene* scene = nullptr;

	bool faint;
	float faintTime;
	glm::vec3 faintColor;

	//std::vector<Key*> bag;

	float health;

	Player(Scene* scene);
	~Player();

	void rotate(int x, int y);
	void fstart();
	void fend();
	void bstart();
	void bend();
	void rstart();
	void rend();
	void lstart();
	void lend();
	void rlstart();
	void rlend();
	void rustart();
	void ruend();
	void rdstart();
	void rdend();
	void rrstart();
	void rrend();
	void jump();
	void AttackStart();
	void AttackStop();
	void Update(float dt);
	void PickUp();
	void CaughtByGhost();
	void HitByBall();
	void OpenDoor();
	void UpdateBag(glm::vec3 nfacing, glm::vec3 nup, float dt,float ry, float rx);
	glm::vec3 GetDirection();
	glm::vec3 GetPosition();
	glm::mat4 GetView();
	glm::vec3 FinalMove(glm::vec3 move, float distance);
	void PickUpFruit();
	void CalculateFaintColor(float dt);
	void TorchONOFF();
	void GunSwitch();
};
#endif