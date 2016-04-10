#ifndef SUBMARINE_H
#define SUBMARINE_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "GetGlut.h"
#include "glut.h"
#include "ObjLibrary/Vector3.h"

#include "ObjLibrary/ObjModel.h"


const float pi = 3.14159;
const float GRAVITY = -0.0000098;	//m/ms^2
const float SUB_MASS = 100.0;  // Kg
const float SUB_BUOYANT_FORCE = 0.000980; //Kgm/ms^2
const float SUB_ENGINE_FORCE = 0.01; //Kgm/ms^2
const float SUB_DRAG = 0.6; //fraction per 1s

const float SUB_TURN_RATE = 0.12; //degrees/ms

const float SUB_BUOYANCY_MIN = 0;
const float SUB_BUOYANCY_MAX = 2;
const float SUB_BUOYANCY_RATE = 0.001; //per ms
const float SUB_SINK_SURFACE = 0.03; //max vertical velocity per ms
const float SUB_TILT_MIN = -10; //degree angle downward 
const float SUB_TILT_MAX = 10; //degree angle upward 

class EyeCam {
public:
	//default constructor
	EyeCam(float x, float y, float z);

	//set look at point trailing eye
	void camera();

	//update position according to velocity for physDelta ms
	//speed is reduced by drag
	void physUpdate(float physDelta);

	//increase speed by engine force for frameDelta ms
	//split force between x and z using yaw
	void engineGo(float frameDelta);
	//rotate the eye
	void rotate(float frameDelta);
	//tilt eye
	void tilt(float frameDelta);
	
	void aiUpdate(float aiDelta);
	void slowaiUpdate();

	//draw EyeCam
	void draw();

	//check ocean boundaries
	bool checkBounds(float axis);

	float xPos; 
	float yPos;
	float zPos;
	float buoyancy;
	float yaw;
	float pitch;
	Vector3 speed;
	
	float v;
	ObjModel eyeModel;
};
#endif