#ifndef SLIME_H
#define SLIME_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "GetGlut.h"
#include "glut.h"
#include "ObjLibrary/Vector3.h"


const float SLIME_MAX_SPEED = 0.020;  // m/ms
const float SLIME_MAX_ACCEL = 0.00001;  // m/ms^2
const float SCHOOL_RANGE = 10.0 ; // m
const float SCHOOL_MIN = 20; //min school population
const float SLIME_MASS = 1.0;  // kg
const float CATCH_RANGE_SQ = 400; //m squared

class Slime {
public:
	//default constructor
	Slime();	
	//test constructor
	Slime(float x, float y, float z);
	//draw slime	
	void printSlime();
	void draw();
	//check distance between eye and slime
	//return true if 20 or less
	//change caught value as side effect
	//ensure slime is ignored if caught
	bool catchSlime(float x, float y, float z);

	void physUpdate(float physDelta);
	void aiUpdate(float aiDelta);	
	void findSchool(Slime *slime, int slimeCount);
	void stop();
	void changeDirection();
	void findYawPitch();
	Vector3 position();


	bool checkDistance(Slime f);		
	
	bool caught;
	Vector3 speed;
	Vector3 targetSpeed;

	int red;
	int green;
	int blue;
	int alpha; 

	float xPos; 
	float yPos;
	float zPos;
	float yaw;
	float pitch;
	float mass;
	bool test;

};
#endif