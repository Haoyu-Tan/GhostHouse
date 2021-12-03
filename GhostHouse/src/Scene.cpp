#include "../include/Scene.h"
#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../include/GlobalVar.h"
#include <math.h>



Scene::Scene() {
	width = 0;
	height = 0;
	start_x = 0;
	start_y = 0;
	goal_x = 0;
	goal_y = 0;
	gameState = 0;
	floorHeight = 0.0;
	colors = {glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1),
			glm::vec3(1, 1, 0), glm::vec3(0, 1, 1)};
	scaleFactor = 0.0f;
	staticFurnitures.clear();
	healthBar = nullptr;
}


Scene::Scene(int w, int h, int s_x, int s_y, int g_x, int g_y) {
	width = w;
	height = h;
	start_x = s_x;
	start_y = s_y;
	goal_x = g_x;
	goal_y = g_y;
	gameState = 0;
	floorHeight = 0.0;
	colors = { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1),
			glm::vec3(1, 1, 0), glm::vec3(0, 1, 1) };
	scaleFactor = 0.0f;
	staticFurnitures.clear();
	healthBar = nullptr;


	walls.clear();
	doors.clear();
	keys.clear();
	pKeys.clear(); // key belongs to player
	fruits.clear();
	//std::vector<Key*> currentKeys;
	colors.clear();
	staticFurnitures.clear();
	whiteGhosts.clear();
	blueGhosts.clear();
	uglyBalls.clear();
	guns.clear();
	fbuls.clear();
	candels.clear();
}

Scene::~Scene() {

	for (int i = 0; i < keys.size(); i++) {
		delete keys[i];
	}

	for (int i = 0; i < doors.size(); i++) {
		delete doors[i];
	}

	for (int i = 0; i < walls.size(); i++) {
		delete walls[i];
	}

	for (int i = 0; i < fruits.size(); i++) {
		delete fruits[i];
	}

	for (int i = 0; i < staticFurnitures.size(); i++) {
		delete staticFurnitures[i];
	}

	if (healthBar != nullptr)
		delete healthBar;

	for (int i = 0; i < whiteGhosts.size(); i++) {
		delete whiteGhosts[i];
	}

	for (int i = 0; i < blueGhosts.size(); i++) {
		delete blueGhosts[i];
	}

	for (int i = 0; i < uglyBalls.size(); i++) {
		delete uglyBalls[i];
	}

	for (int i = 0; i < fbuls.size(); i++) {
		delete fbuls[i];
	}

	for (int i = 0; i < guns.size(); i++) {
		delete guns[i];
	}
}


glm::vec3 Scene::StartPosition() {
	return glm::vec3(-scaleFactor * start_x, 1.75, scaleFactor * start_y);
}

void Scene::printSceneInfo() {
	printf("Map width: %d, heigh: %d\n", width, height);
	printf("Start Location x: %d, y: %d\n", start_x, start_y);
	printf("Goal Location x: %d, y: %d\n", goal_x, goal_y);
	printf("Game state: %d\n", gameState);
	
	printf("print wall information:\n");
	for (int i = 0; i < walls.size(); i++) {
		Wall* w = walls[i];
		printf("wall %d, location x: %d, y: %d\n", i, w->pos_x, w->pos_y);
	}

	printf("print door information:\n");
	for (int i = 0; i < doors.size(); i++) {
		Door* door = doors[i];
		printf("door %d, location x: %d, y: %d, index: %c\n", i, door->pos_x, door->pos_y, door->index);
		printf("color: %f, %f, %f\n", door->color.r, door->color.g, door->color.b);
	}

	printf("print key information:\n");
	for (int i = 0; i < keys.size(); i++) {
		Key* key = keys[i];
		printf("key %d, location x: %d, y: %d, index: %c\n", i, key->pos_x, key->pos_y, key->index);
		printf("color: %f, %f, %f\n", key->color.r, key->color.g, key->color.b);
	}

	printf("print fruit information:\n");
	for (int i = 0; i < fruits.size(); i++) {
		Fruit* fruit = fruits[i];
		printf("fruit %d, location x: %d, y: %d\n", i, fruit->pos_x, fruit->pos_y);
	}

	printf("print static furniture:\n");
	for (int i = 0; i < staticFurnitures.size(); i++) {
		StaticFurniture* sf = staticFurnitures.at(i);
		printf("static furniture: %d, type: %d, location x: %f, y: %f\n", i, sf->modelType, sf->pos_x, sf->pos_y);
	}
}


void Scene::drawDGeometry(int shaderProgram, GLuint vao, GLuint vbo) {

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	GLint uniColor = glGetUniformLocation(shaderProgram, "inColor");
	GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");

	for (int i = 0; i < whiteGhosts.size(); i++) {
		WhiteGhost* wg = whiteGhosts.at(i);
		/*
		glm::mat4 model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(wg->position.x, wg->position.y, wg->position.z));
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

		glUniform1i(uniTexID, -1);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));


		//temp
		//printf("Total vert is %i \n",wg->totalVert);
		//printf("Length is %i \n", wg->totalVert) * 22 * sizeof(float));
		//printf("My vert size is %i\n", wg->myVert.size());
		glBufferData(GL_ARRAY_BUFFER, (wg->totalVert) * 22 * sizeof(float), (wg->myVert).data(), GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, wg->totalVert);*/
		
		//Frame *fr = ((dynamicModels.at(0)->animations).at(0)->frames).at(frameIndex);

		/*
		glm::mat4 model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(wg->position.x, wg->position.y, wg->position.z));
		model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
		*/

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(wg->modelMatrix));

		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, wg->animeSI, wg->totalVert);
		
		
	}

	for (int i = 0; i < blueGhosts.size(); i++) {
		BlueGhost* bg = blueGhosts.at(i);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(bg->modelMatrix));

		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, bg->animeSI, bg->totalVert);


	}
	for (int i = 0; i < uglyBalls.size(); i++) {
		UglyBall* ub = uglyBalls.at(i);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(ub->modelMatrix));

		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, ub->animeSI, ub->totalVert);


	}
	for (int i = 0; i < guns.size(); i++) {
		Gun* gun = guns.at(i);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(gun->modelm));

		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, gun->animeSI, gun->totalVert);


	}

	for (int i = 0; i < fbuls.size(); i++) {
		FBullet* fbu = fbuls.at(i);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(fbu->modelMatrix));

		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, fbu->animeSI, fbu->totalVert);


	}

	if (pgun->renderS) {
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(pgun->modelMatrix));

		glUniform1i(uniTexID, 3);
		glDrawArrays(GL_TRIANGLES, pgun->animeSI, pgun->totalVert);

		for (int i = 0; i < pgun->bulletNumber; i++) {
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(pgun->GetBMatrix(i)));
			glUniform1i(uniTexID, 3);
			glDrawArrays(GL_TRIANGLES, pgun->banimeSI, pgun->btotalVert);
		}
	}


	glBindVertexArray(0);
}





void Scene::drawGeometry(int shaderProgram, std::vector<Model*> staticModels, 
	float timePast, float scaleFactor, GLuint vao, GLuint vbo[]) {

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	float realSF = scaleFactor / 2.0;

	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	GLint uniColor = glGetUniformLocation(shaderProgram, "inColor");
	GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");

	glm::mat4 model;

	//draw floor
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			model = glm::mat4(1);

			model = glm::translate(model, glm::vec3(-j * scaleFactor, 0, i * scaleFactor));
			model = glm::scale(model, glm::vec3(realSF, realSF, realSF));
			//model = glm::translate(model, glm::vec3(0, -0.6001, 0));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

			glUniform1i(uniTexID, -1);
			glDrawArrays(GL_TRIANGLES, (staticModels.at(floorr))->startIndex, (staticModels.at(floorr))->totalVert);
		}

	}

	//draw ceiling
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			model = glm::mat4(1);

			model = glm::translate(model, glm::vec3(-j * scaleFactor, 4.01, i * scaleFactor));
			model = glm::scale(model, glm::vec3(realSF, realSF, realSF));
			//model = glm::translate(model, glm::vec3(0, -0.6001, 0));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

			glUniform1i(uniTexID, -1);
			glDrawArrays(GL_TRIANGLES, (staticModels.at(floorr))->startIndex, (staticModels.at(floorr))->totalVert);
		}
	}

	//draw wall
	for (int i = 0; i < walls.size(); i++) {
		Wall* currWall = walls.at(i);
		model = glm::mat4(1);

		model = glm::translate(model,currWall->position);
		//model = glm::scale(model, glm::vec3(realSF, scaleFactor, scaleFactor));
		model = glm::scale(model,currWall->scale);
		//model = glm::rotate(model, (currWall->rotateAngle).x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (currWall->rotateAngle).y, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, (currWall->rotateAngle).z, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, (staticModels.at(wall))->startIndex, (staticModels.at(wall))->totalVert);
	}

	//draw door
	for (int i = 0; i < doors.size(); i++) {
		Door* cdoor = doors.at(i);

		model = glm::mat4(1);

		printf("current height is: %f\n", float(cdoor->currentHeight));
		model = glm::translate(model, glm::vec3(-scaleFactor * (cdoor->pos_x), cdoor->currentHeight, scaleFactor * cdoor->pos_y));
		model = glm::scale(model, glm::vec3(realSF, realSF, realSF));
		model = glm::rotate(model, cdoor->angle, glm::vec3(0.0f, 1.0f, 0.0f));
		
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, (staticModels.at(door))->startIndex, (staticModels.at(door))->totalVert);
	}
	
	for (int i = 0; i < keys.size(); i++) {
		Key* mkey = (keys).at(i);
		glm::mat4 model = mkey->GetModelMatrix();
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//glUniform3fv(uniColor, 1, glm::value_ptr(key->color));
		//glUniform3fv(uniColor, 1, glm::value_ptr(door->color));
		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, (staticModels.at(key))->startIndex, (staticModels.at(key))->totalVert);
	}

	for (int i = 0; i < pKeys.size(); i++) {
		Key* mkey = pKeys.at(i);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(mkey->GetModelMatrix()));

		glUniform1i(uniTexID, 3);
		glDrawArrays(GL_TRIANGLES, (staticModels.at(key))->startIndex, (staticModels.at(key))->totalVert);


	}
	
	for (int i = 0; i < candels.size(); i++) {
		Candel* cd = candels.at(i);
		sModelType mt = cd->modelType;
		model = glm::mat4(1);
		model = glm::translate(model, cd->position);
		model = glm::translate(model, cd->ranslateDist);
		model = glm::rotate(model, cd->rotateAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cd->rotateAngle.x, glm::vec3(1.0, 0.0f, 0.0f));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, (staticModels.at(mt))->startIndex, (staticModels.at(mt))->totalVert);
	}


	for (int i = 0; i < staticFurnitures.size(); i++) {
		StaticFurniture *sf = staticFurnitures.at(i);
		sModelType mt = sf->modelType;

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-scaleFactor * (sf->pos_x), 0.0, scaleFactor * (sf->pos_y)));

		model = glm::translate(model, sf->ranslateDist);
		model = glm::rotate(model, sf->rotateAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, sf->rotateAngle.x, glm::vec3(1.0, 0.0f, 0.0f));
		//model = glm::rotate(model, sf->rotateAngle.z, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(staticModels.at(mt)->translate_x, staticModels.at(mt)->translate_y, staticModels.at(mt)->translate_z));
		
		if (mt == stand) {
			model = glm::rotate(model, 90 * 3.14f / 180.0f, glm::vec3(0.0, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));
		}
		else if (mt == table) {
			model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));
		}
		else if (mt == torch) {
			model = glm::scale(model, glm::vec3(1.3, 1.3, 1.3));
		}
		else if (mt == desk) {
			model = glm::translate(model, glm::vec3(0.0, 0.1, 0.0));
			model = glm::rotate(model, 90 * 3.14f / 180.0f, glm::vec3(0.0, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.5, 2.5, 1.5));
		}


		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));


		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, (staticModels.at(mt))->startIndex, (staticModels.at(mt))->totalVert);
	}
	
	
	
	//render health bar
	sModelType hbm = health_base;
	sModelType hbarm = health_bar;

	//health base
	
	model = glm::mat4(1); 

	model = glm::translate(model,healthBar->playerPos);
	model = glm::rotate(model, healthBar->rotateAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, healthBar->rotateAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(healthBar->transDist.x, healthBar->transDist.y, healthBar->transDist.z));
	model = glm::scale(model, glm::vec3(0.01, 0.005, 0.01));
	
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(uniTexID, 2);
	glDrawArrays(GL_TRIANGLES, (staticModels.at(hbm))->startIndex, (staticModels.at(hbm))->totalVert);
	
	
	model = glm::mat4(1);
	model = glm::translate(model, healthBar->playerPos2);
	model = glm::rotate(model, healthBar->rotateAngle.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, healthBar->rotateAngle.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(healthBar->transDist.x, healthBar->transDist.y, healthBar->transDist.z));
	model = glm::scale(model, glm::vec3(0.01f*healthBar->barSF, 0.005, 0.01));
	
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(uniTexID, 2);
	glDrawArrays(GL_TRIANGLES, (staticModels.at(hbarm))->startIndex, (staticModels.at(hbarm))->totalVert);






	glBindVertexArray(0);
	/*
	//draw bookcase
	glm::mat4 model = glm::mat4(1);
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(-scaleFactor * 0, 1.0, scaleFactor * 1));
	model = glm::translate(model, glm::vec3(staticModels.at(1)->translate_x, staticModels.at(1)->translate_y, staticModels.at(1)->translate_z));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	
	glUniform1i(uniTexID, -1);
	glDrawArrays(GL_TRIANGLES, (staticModels.at(1))->startIndex, (staticModels.at(1))->totalVert);


	//draw sheep
	model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(-scaleFactor * -1, 1.0, scaleFactor * 1));
	model = glm::rotate(model, timePast * 3.14f / 4, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(staticModels.at(2)->translate_x, staticModels.at(2)->translate_y, staticModels.at(2)->translate_z));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(uniTexID, -1);
	glDrawArrays(GL_TRIANGLES, (staticModels.at(2))->startIndex, (staticModels.at(2))->totalVert);
	
	*/

	

	/*
	int numWallVert = doorStart - wallStart;
	//cout << "wall vertex number is: " << numWallVert << endl;
	int wallSize = (walls).size();

	for (int i = 0; i < wallSize; i++) {
		int wall_x = (walls).at(i)->pos_x;
		int wall_y = (walls).at(i)->pos_y;
		//printf("x is %d, y is %d\n", wall_x, wall_y);
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-scaleFactor * wall_x, 0, scaleFactor * wall_y));
		model = glm::scale(model, glm::vec3(realSF, realSF, realSF));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(uniTexID, 1);
		glDrawArrays(GL_TRIANGLES, wallStart, numWallVert);
	}

	GLint uniColor = glGetUniformLocation(shaderProgram, "inColor");
	int numDoorVert = keyStart - doorStart;
	int doorSize = (doors).size();

	for (int i = 0; i < doorSize; i++) {
		Door* door = (doors).at(i);
		int door_x = door->pos_x;
		int door_y = door->pos_y;

		//set translate
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-scaleFactor * door_x, door->currentHeight, scaleFactor * door_y));
		model = glm::scale(model, glm::vec3(realSF, realSF, realSF));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		//set color
		glUniform3fv(uniColor, 1, glm::value_ptr(door->color));
		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, doorStart, numDoorVert);

		//if ((door->currentHeight) - (door->highest) <= 0.0002f) {
			//(scene->doors).erase((scene->doors).begin()+i);
		//}

	}

	int numKeyVert = goalStart - keyStart;
	int keySize = (keys).size();
	for (int i = 0; i < keySize; i++) {
		Key* key = (keys).at(i);
		glm::mat4 model = key->GetModelMatrix();
		//model = glm::translate(model, glm::vec3(-scaleFactor * key->pos_x, 0, scaleFactor * key->pos_y));
		//model = glm::scale(model, glm::vec3(realSF, realSF, realSF));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniform3fv(uniColor, 1, glm::value_ptr(key->color));
		//glUniform3fv(uniColor, 1, glm::value_ptr(door->color));
		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, keyStart, numKeyVert);
	}

	int ks = (p->bag).size();
	for (int i = 0; i < ks; i++) {
		Key* key = (p->bag).at(i);
		glm::mat4 model = key->GetModelMatrix();
		//model = glm::translate(model, glm::vec3(-scaleFactor * key->pos_x, 0, scaleFactor * key->pos_y));
		//model = glm::scale(model, glm::vec3(realSF, realSF, realSF));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniform3fv(uniColor, 1, glm::value_ptr(key->color));
		//glUniform3fv(uniColor, 1, glm::value_ptr(door->color));
		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, keyStart, numKeyVert);
	}

	int numGoalVert = fruitStart - goalStart;
	glm::mat4 model = glm::mat4(1);

	model = glm::translate(model, glm::vec3(-(goal_x) * scaleFactor, 1.0, (goal_y) * scaleFactor));
	model = glm::rotate(model, timePast * 3.14f / 2, glm::vec3(0.0f, 1.0f, 1.0f));
	model = glm::rotate(model, timePast * 3.14f / 4, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(realSF / 5.0, realSF / 2.5, realSF / 5.0));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	//set color
	glm::vec3 goalCol = glm::vec3(1.0, 1.0, 1.0);
	glUniform3fv(uniColor, 1, glm::value_ptr(goalCol));
	glUniform1i(uniTexID, -1);
	glDrawArrays(GL_TRIANGLES, goalStart, numGoalVert);


	int numFruitVert = total - fruitStart;
	int fruitSize = (fruits).size();

	for (int i = 0; i < fruitSize; i++) {
		Fruit* fruit = (fruits).at(i);
		//if (!(fruit->picked)) {
		int fruit_x = fruit->pos_x;
		int fruit_y = fruit->pos_y;

		//set translate
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-scaleFactor * fruit_x, 1.0, scaleFactor * fruit_y));
		model = glm::rotate(model, timePast * 3.14f / 2, glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::rotate(model, timePast * 3.14f / 4, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(realSF / 2, realSF / 2, realSF / 2));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		//set color
		glUniform3fv(uniColor, 1, glm::value_ptr(fruit->color));
		glUniform1i(uniTexID, -1);
		glDrawArrays(GL_TRIANGLES, fruitStart, numFruitVert);

	}
	*/
}

void Scene::Update(float dt, glm::vec3 pp, glm::vec3 pd) {
	for (int i = 0; i < whiteGhosts.size(); i++) {
		whiteGhosts.at(i)->Update(dt, pp, pd);
	}

	for (int i = 0; i < blueGhosts.size(); i++) {
		blueGhosts.at(i)->Update(dt, pp, pd, &uglyBalls);
	}
	for (int i = uglyBalls.size() - 1; i >= 0; i--) {
		uglyBalls.at(i)->Update(dt, pp, &walls, &staticFurnitures);
		if (uglyBalls.at(i)->destroyed) {
			delete uglyBalls[i];
			uglyBalls.erase(uglyBalls.begin() + i);
		}
	}
	for (int i = fbuls.size() - 1; i >= 0; i--) {
		fbuls.at(i)->Update(dt, &walls, &staticFurnitures, &whiteGhosts, &blueGhosts, &doors);
		if (fbuls.at(i)->destroyed) {
			delete fbuls[i];
			fbuls.erase(fbuls.begin() + i);
		}
	}
	for (int i = 0; i < doors.size(); i++) {
		printf("before update, height: %f\n", (doors.at(i))->currentHeight);
		doors.at(i)->update(dt, pp);
		
	}
	for (int i = 0; i < guns.size(); i++) {
		//printf("before update, height: %f\n", (doors.at(i))->currentHeight);
		guns.at(i)->Update(dt);

	}

	for (int i = 0; i < keys.size(); i++) {
		//printf("before update, height: %f\n", (doors.at(i))->currentHeight);
		keys.at(i)->update(dt);

	}
}