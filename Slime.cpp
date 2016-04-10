#include "Slime.h"

using namespace std;

//slime constructor
Slime::Slime(){
	// choose random positions
	// excluding values to close to the boundary
	xPos = rand() % 990 + 5;
	yPos = -5;
	zPos = rand() % 990 + 5;

	yaw = 0;		//initialise orientation
	pitch = 0;		//initialise orientation
	speed.setZero();	//initialise speed
	changeDirection();	//choose random direction

	do{	//random color
		red = rand() % 255;
		green = rand() % 255;
	//ensure random color is not dark
	}while (red + green < 255);
	blue = 0;
	
	//initialise caught value
	caught = false;
	//define test variable
	test = false;
	//initialise mass
	mass = SLIME_MASS;
}
//draw slime
void Slime::draw(){
	if (!caught){ //ignore caught slime
		glPushMatrix();	//{
			glTranslatef(xPos, yPos, zPos);		//draw location
			glPushMatrix();	//{
				/*
				glBegin(GL_LINES);
					glVertex3f(0,0,0);
					glVertex3f(speed.x * 300.0, speed.y * 300.0, speed.z * 300.0);
				glEnd();
				//*/
				glRotatef(yaw, 0.0, 1.0, 0.0);				//orient to motion
				glRotatef(pitch, 1.0, 0.0, 0.0);				//orient to motion
				printSlime();
			glPopMatrix(); //}
		glPopMatrix(); //}

	}
}


///////////////////physUpdate///////////////////
void Slime::physUpdate(float physDelta){
	if (speed.isNormGreaterThan(SLIME_MAX_SPEED))
		speed.setNorm(SLIME_MAX_SPEED);		//scale motion to speed
	xPos += speed.x*physDelta;
	//yPos += speed.y*physDelta;
	zPos += speed.z*physDelta;	
	if (xPos >= 1000) speed.x = -abs(speed.x);
	if (xPos <= 0) speed.x = abs(speed.x);
	if (yPos >= 1000) speed.y = -abs(speed.y);
	if (yPos <= 0) speed.y = abs(speed.y);
	if (zPos >= 1000) speed.z = -abs(speed.z);
	if (zPos <= 0) speed.z = abs(speed.z);
	findYawPitch();
	//write test slime to console
	if (test) cout << xPos << ", " << yPos << ", " << zPos << " ==> " << speed.getNorm() << endl; 
}
///////////////////aiUpdate///////////////////
void Slime::aiUpdate(float aiDelta){
	Vector3 changeSpeed = targetSpeed - speed; //find difference vector	
	float maxChange = SLIME_MAX_ACCEL * aiDelta;
	if (changeSpeed.isNormGreaterThan(maxChange)) 
		changeSpeed.setNorm(maxChange); //scale motion to accel rate
	speed += changeSpeed; 
	//write test slime to console 
	if (test) 
		cout << changeSpeed.getNorm() << " - " << targetSpeed.getNorm() << endl; 
}
///////////////////slowaiUpdate///////////////////
void Slime::findSchool(Slime *slime, int slimeCount){
	int friends = 0;
	for (int i = 0; i < slimeCount || friends == SCHOOL_MIN; i++){
		if (checkDistance(slime[i]))	friends ++;
	}//*/	
	if (friends == SCHOOL_MIN) stop();
	else changeDirection();
	
}
bool Slime::checkDistance(Slime f){
	if (!f.caught){
		Vector3 v = position() - f.position();
		if (v.getNormSquared() <= SCHOOL_RANGE*SCHOOL_RANGE)
			return true;
	}
	return false;
}

void Slime::changeDirection(){
	//if (targetSpeed != Vector3::ZERO){
		targetSpeed = Vector3::getRandomUnitVector();
		targetSpeed.setNorm(SLIME_MAX_SPEED);
	//}
}
////////////////////YawPitch////////////////////
void Slime::findYawPitch(){
	Vector3 v = speed;
	float pi = 3.14159;
	//
	if (v.z < 0) 
		yaw = (pi + atan(v.x / v.z)) * 180/pi;
	else if (v.z > 0) 
		yaw = atan (v.x / v.z) * 180/pi;
	else if (v.z == 0){
		if (v.x > 0) yaw = 90;
		else if (v.x < 0) yaw = 270;
	}

	float r = sqrt(v.x*v.x + v.z*v.z);
	if (r != 0) 
		pitch = -atan(v.y/r) * 180/pi;
	else {
		if (v.y < 0) pitch = 90; 
		else if (v.y > 0) pitch = 270;
	}

}

void Slime::stop(){
	targetSpeed.setZero();
	//red = 128;
	//green = 0;
	//blue = 128;
}

Vector3 Slime::position(){
	return Vector3(xPos, yPos, zPos);
}

bool Slime::catchSlime(float x, float y, float z){
	if (!caught){ //ignore caught slime
		Vector3 v = Vector3(x - xPos, y - yPos, z - zPos);
		//if sqrt( (x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2) < 20
		if (v.getNormSquared() <= CATCH_RANGE_SQ){
			//slime is caught
			caught = true;
			return true;
		}
	//write test slime to console 
	if (test) 
		cout << v.getNorm() << endl; 
	}
	return false;
}

void Slime::printSlime(){
	glPushMatrix();	//{
		glColor4ub(red/1.2, green/1.2, 0, alpha);	//darker back fin
		glPushMatrix();	//{
			glutSolidSphere(3, 10, 10); 			//cone slime	
		glPopMatrix(); //}
	glPopMatrix();	//}
	
}

//test slime constructor
Slime::Slime(float x, float y, float z){
	xPos = x;
	yPos = y;
	zPos = z;
	yaw = 90;
	red = 0;
	green = 0;
	blue = 0;
	caught = false;
	test = false;
	yaw = 0;
	pitch = 0;
	speed.setZero();
	changeDirection();
}


