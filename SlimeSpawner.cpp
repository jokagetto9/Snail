#include "SlimeSpawner.h"

using namespace std;

//SlimeSpawner constructor
SlimeSpawner::SlimeSpawner(){

	srand (time(NULL));		//random seed 
	slimeCount = rand() % RAND_SIZE + START_SIZE; //randomly choose between 100-200 slime
	for (int i = 0; i < slimeCount; i++){
		slime[i] = Slime();
	}
	
	f = Slime(500, 500, 30);

	nextSlime = 0;
	slimeCaught = 0;
}

//draw SlimeSpawner
void SlimeSpawner::drawSlime(){
	//draw slime
	for (int i = 0; i < slimeCount; i++){
		slime[i].draw();
	}
	f.draw();
}

//check catchSlime
void SlimeSpawner::catchSlime(float x, float y, float z){
	//check is the eye is within range to gain a point
	for (int i = 0; i < slimeCount; i++){
		if (slime[i].catchSlime(x, y, z)){
			slimeCaught += 1;  //gain a point
		}
	}
	f.catchSlime(x, y, z);
}

//draw SlimeSpawner
void SlimeSpawner::physUpdate(float physDelta){
	for (int i = 0; i < slimeCount; i++){
		if (!slime[i].caught)
			slime[i].physUpdate(physDelta);
	}
	f.physUpdate(physDelta);
	
}
void SlimeSpawner::aiUpdate(float aiDelta, float slowai){
	for (int i = 0; i < slimeCount; i++)
		if (!slime[i].caught)
			slime[i].aiUpdate(aiDelta);
	f.aiUpdate(aiDelta);
	int friends = 0;
	int slimeChecked = 0;
	int slimeToCheck = ceil(slowai*(slimeCount-slimeCaught));
	while (slimeChecked < slimeToCheck){
		if (slime[nextSlime].caught){
			//cycle through slime thoughts
			if (nextSlime+1 >= slimeCount) nextSlime = 0;
			else nextSlime++;	
		}else{
			for (int j = 0; j < slimeCount && friends != SCHOOL_MIN; j++){
				if (j != nextSlime){
					if (slime[nextSlime].checkDistance(slime[j])) friends++;
				}
			}	
			if (friends == SCHOOL_MIN) slime[nextSlime].stop();
			else slime[nextSlime].changeDirection();
			if (nextSlime == slimeCount-1) f.changeDirection(); 

			//cycle through slime thoughts
			if (nextSlime+1 == slimeCount) nextSlime = 0;
			else nextSlime++;
			slimeChecked++;
		}
	}
}



bool SlimeSpawner::win(){
	return slimeCaught >= 10;
}

void SlimeSpawner::increaseSlime(){
	if (slimeCount < 4000){
		slimeCount++;
		slime[slimeCount] = Slime();
	}
}
void SlimeSpawner::decreaseSlime(){
	if (slimeCount > 100){
		slimeCount--;
	}
	
}	
	
	

/*SlimeSpawner deconstructor
SlimeSpawner::~SlimeSpawner(){
	//delete [] slime;		//clear slime array
}//*/



