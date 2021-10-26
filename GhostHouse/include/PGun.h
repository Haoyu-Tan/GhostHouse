#ifndef PGUN_INCLUDED
#define PGUN_INCLUDED

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/glm.hpp"
#include "MyStruct.h"
#include "FBullet.h"

class PGun {
public:
	glm::mat4 modelMatrix;
	DModel* dm;
	
	float reloadTime;
	bool renderS;
	float size;
	int animeSI; // anime start index
	int totalVert;

	int action;
	int bulletNumber;
	float angleX;


	int banimeSI;
	int btotalVert;
	int bangle;
	float bsize;
	glm::vec3 pright;
	glm::mat4 bmmatrix;


	PGun(DModel* d);
	

	~PGun();


	void Shot(glm::vec3 pos, glm::vec3 fdir, std::vector<FBullet*>* fbuls, float ry, float rx);
	void UseGunOrNot();
	void Reload(float dt);
	void UpdateByPlayer(glm::vec3 ppos, glm::vec3 pf, glm::vec3 pup, glm::vec3 nright,
		 float ry, float rx, glm::vec3 facing, float dt, bool attack, std::vector<FBullet*>* fbuls);
	glm::mat4 GetBMatrix(int index);
};



#endif
