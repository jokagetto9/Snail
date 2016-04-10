#include "EyeCam.h"

//EyeCam constructor
EyeCam::EyeCam(float x, float y, float z){
	xPos = x; yPos = y; zPos = z;	
	yaw = 0;
	pitch = 0;
	buoyancy = 1;
	speed.setZero();
	
}

//draw EyeCam
void EyeCam::draw(){
	glPushMatrix();	//{
		//glColor3ub(255, 255, 0); //Yellow submarine
		glTranslatef(xPos, yPos, zPos);	//offset sub position
		glRotatef(yaw, 0.0, 1.0, 0.0);				//orient to motion
		glRotatef(-pitch, 1.0, 0.0, 0.0);
		glPushMatrix();	
			glRotatef(-90, 0.0, 1.0, 0.0);	
			//subModel.draw();
		glPopMatrix();			
	glPopMatrix(); //}

}

void drawSub(){
		/* Simple sub
		glPushMatrix();	//{
			glScalef(1.0, 1.0, 5.0);				//elongate sphere
			glutSolidSphere(3, 20, 20); 		
		glPopMatrix(); //{*/
}

void EyeCam::physUpdate(float physDelta){
	speed.x *= pow(SUB_DRAG, physDelta/1000);	//slow by drag per sec
	speed.z *= pow(SUB_DRAG, physDelta/1000);	//slow by drag per sec

	//gravity vs buoyancy
	if (abs(speed.y) < SUB_SINK_SURFACE)	
		speed.y += physDelta*(buoyancy * SUB_BUOYANT_FORCE/SUB_MASS + GRAVITY);

	//move according to speed for physDelta ms
	xPos += speed.x*physDelta;	
	yPos += speed.y*physDelta;
	zPos += speed.z*physDelta;	
	
	//check boundaries
	if (xPos >= 1000) {xPos = 1000; speed.x = 0;}
	if (xPos <= 0) {xPos = 0; speed.x = 0;}
	if (yPos >= 1000) {yPos = 1000; speed.y = 0;}
	if (yPos <= 0) {yPos = 0; speed.y = 0;}
	if (zPos >= 1000) {zPos = 1000; speed.z = 0;}
	if (zPos <= 0) {zPos = 0; speed.z = 0;}
}

void EyeCam::engineGo(float frameDelta){
	//determine resulting engine velocity
	float v = SUB_ENGINE_FORCE/SUB_MASS * frameDelta;
	speed.x += v * sin(yaw * pi/180);
	speed.z += v * cos(yaw * pi/180);	
	
} 
void EyeCam::rotate(float frameDelta){
	//ensure yaw stays between 360 and -360
	if (yaw >= 360) yaw -= 360;
	if (yaw <= -360) yaw += 360;
	//rotate by turn rate
	yaw += SUB_TURN_RATE * frameDelta;

	float momentum = speed.getNormXZ(); 
	speed.x = momentum * sin(yaw * pi/180);
	speed.z = momentum * cos(yaw * pi/180);	

}
void EyeCam::tilt(float frameDelta){
	//change buoyancy if it is range
	if (buoyancy <= SUB_BUOYANCY_MAX && buoyancy >= SUB_BUOYANCY_MIN) 
		buoyancy += SUB_BUOYANCY_RATE * frameDelta;
	//fix buoyancy to maximum
	if (buoyancy > SUB_BUOYANCY_MAX){
		buoyancy = SUB_BUOYANCY_MAX; pitch = SUB_TILT_MAX;
	}
	//fix buoyancy to minimum
	if (buoyancy < SUB_BUOYANCY_MIN){
		buoyancy = SUB_BUOYANCY_MIN; pitch = SUB_TILT_MIN;
	}
	//set pitch according to tilt
	if (buoyancy > 1.0 && buoyancy < SUB_BUOYANCY_MAX) 
		pitch = (buoyancy-1)*10; 
	if (buoyancy < 1.0 && buoyancy > SUB_BUOYANCY_MIN)
		pitch = (buoyancy-1)*10;
	//disable continuous momentum
	if (buoyancy > 1.0 && speed.y < 0) speed.y = 0;
	if (buoyancy < 1.0 && speed.y > 0) speed.y = 0;
}



///////////////////lookatpoint///////////////////////
void EyeCam::camera(){
	float offsetX = 30 * sin(yaw * pi/180);
	float offsetZ = 30 * cos(yaw * pi/180);
	float offsetY = 30 * tan(pitch * pi/180);
	gluLookAt(
		xPos, yPos, zPos,		// x,y,z of eye
		xPos, 0, zPos+50,	// x,y,z of look at point
		0.0, 1.0, 0.0);			// x,y,z of up vector
	
}





