#ifndef MY_STRUCT_INCLUDED
#define MY_STRUCT_INCLUDED

#include <limits>
#include <vector>

//illum:
//2. Highlight on

struct Material {
	char mName[128];
	float Ns;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 Ke;
	float Ni;
	float d;
	int illum;
	Material() {
		Ns = 0.0;
		Ka = glm::vec3(0.0, 0.0, 0.0);
		Kd = glm::vec3(0.0, 0.0, 0.0);
		Ks = glm::vec3(0.0, 0.0, 0.0);
		Ke = glm::vec3(0.0, 0.0, 0.0);
		Ni = 0.0;
		d = 0.0;
		illum = 0;
	}
};

struct mData {
	glm::vec3 position;
	glm::vec3 normal;
	mData() {}
};



struct Part {
	int startIndex;
	int totalVert;
	bool s;
	Part() {
		startIndex = 0;
		totalVert = 0;
		s = false;
	}
	/*
	int numFace;
	int numVert;
	Material m;
	bool s;
	std::vector<mData> data; // a line of data pass to vbo
	*/
};

struct Model {
	int startIndex; //start index in vbo
	int totalVert; //total number of vertices data (all information of a vert together count as 1)
	std::vector<Material> materials;
	std::vector<Part> parts;
	float min_x, max_x, min_y, max_y, min_z, max_z;
	float translate_x, translate_y, translate_z;
	Model() {
		min_x = std::numeric_limits<double>::infinity();;
		max_x = -std::numeric_limits<double>::infinity();;
		min_y = std::numeric_limits<double>::infinity();;
		max_y = -std::numeric_limits<double>::infinity();;
		min_z = std::numeric_limits<double>::infinity();;
		max_z = -std::numeric_limits<double>::infinity();;
		startIndex = 0;
		totalVert = 0;
		materials.clear();
		parts.clear();
	}

	void calTrans() {
		translate_x = -((max_x + min_x) / 2.0);
		translate_y = -((max_y + min_y) / 2.0);
		translate_z = -((max_z + min_z) / 2.0);
	}
};

struct Frame{
	int startIndex;
	int totalVert;
	float updateTime;
	std::vector<Material> materials;
	std::vector<Part> parts;

	Frame() {
	}

	~Frame() {}
};

struct Animation{
	int startIndex;
	std::vector<Frame*> frames;
	Animation() {
	}

	~Animation() {
		for (int i = 0; i < frames.size(); i++) {
			delete frames.at(i);
		}
	}
};

//dynamic model
struct DModel {
	int startIndex;
	//std::vector<Material> materials;
	std::vector<Animation*> animations;
	DModel() {
	}

	~DModel() {
		for (int i = 0; i < animations.size(); i++) {
			delete animations.at(i);
		}
	}
};

struct HealthBar {
	glm::vec3 basePos;
	glm::vec3 barPos;

	glm::vec3 originFacing;
	glm::vec3 rotateAngle;
	glm::vec3 transDist;
	//glm::vec3 transDist2;
	glm::vec3 playerPos;
	glm::vec3 playerPos2;

	float barSF;

	HealthBar() {
		basePos = glm::vec3(0.0, 0.0, 0.0);
		barPos = glm::vec3(0.0, 0.0, 0.0);
		originFacing = glm::vec3(0.0, 0.0, -1.0);
		rotateAngle = glm::vec3(0.0, 0.0, 0.0);
		transDist = glm::vec3(0.0, 0.0, 0.0);
		//transDist2 = glm::vec3(0.0, 0.0, 0.0);
		playerPos = glm::vec3(0.0, 0.0, 0.0);
		playerPos2 = glm::vec3(0.0, 0.0, 0.0);
		barSF = 1.0;
	}

	~HealthBar() {
	}

	void update(glm::vec3 ppos, glm::vec3 pf, glm::vec3 pup, int phealth,float ry, float rx,glm::vec3 facing) {
		//printf("ppos: %f, %f, %f; pf: %f, %f, %f; pup: %f, %f, %f\n", ppos.x, ppos.y, ppos.z, pf.x, pf.y, pf.z, pup.x, pup.y, pup.z);
		
		//calculate the tranlation of base, move with player's camera
		glm::vec3 nright = glm::normalize(glm::cross(pf, pup));
		playerPos = ppos + glm::normalize(pup) * 0.055f + nright * 0.04f ;
		playerPos2 = ppos + glm::normalize(pup) * 0.055f + nright * 0.04f - 0.00005f * pf;
		glm::vec3 center = facing * 0.15f;
		transDist = center;

		originFacing = glm::normalize(originFacing);
		rotateAngle = glm::vec3(0.0, 0.0, 0.0);
		//glm::vec3 tempA = -pf;
		//glm::vec3 temp = glm::normalize(glm::vec3);
		glm::vec3 temp = (glm::vec3(-pf.x, 0.0, -pf.z));;
		float dp = temp.x * originFacing.x + temp.z * originFacing.z;
		float det = temp.x * originFacing.z - temp.z * originFacing.x;
		rotateAngle.y = rx;
		rotateAngle.x = ry;

		//calculate the translation of bar, need to interpolate
		//phealth = 50.0f;
		barSF = phealth / 100.0f;
	}
};

#endif
