#ifndef SLIMESPAWNER_H
#define SLIMESPAWNER_H

#include "ObjLibrary/Vector3.h"
#include "Slime.h"

const float START_SIZE = 800;
const int RAND_SIZE = 201;

class SlimeSpawner {
public:
	//default constructor
	SlimeSpawner();	

	//draw all slime
	void drawSlime();
	//update physics for all slime
	void physUpdate(float physDelta);
	//update ai for all slime
	void aiUpdate(float aiDelta, float slowai);
	//test catch range for all slime
	void catchSlime(float x, float y, float z);
	
	void increaseSlime();

	void decreaseSlime();


	//check win condition
	bool win();


	int slimeCaught;			//total slime caught
	int slimeCount;			//total number of slime	
	int nextSlime;			//next slime to think
	Slime slime [4000];			//array of maximum 4000 slime
	Slime f;
	
};
#endif