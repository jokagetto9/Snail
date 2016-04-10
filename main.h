#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "GetGlut.h"
#include "glut.h"

#include "Slime.h"
#include "SlimeSpawner.h"
#include "EyeCam.h"

#include "ObjLibrary/Vector3.h"
#include "ObjLibrary/Sleep.h"
#include "ObjLibrary/TextureManager.h"
#include "ObjLibrary/SpriteFont.h"
#include "ObjLibrary/DisplayList.h"
#include "ObjLibrary/ObjModel.h"

using namespace std;

// prototypes
void init(void);
void update(void);

void display(void);
void reshape(int w, int h);

void changeSeaColor();
void drawSkyAndFloor();
void drawText();
void winScreen();
void printMaxim(float xpos, float yPos, float zPos);


void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void keyFunctions();
void specialKey(int special_key, int x, int y);
void specialKeyUp(int special_key, int x, int y);
void specialKeyFunctions();

void readHeightMap();

//constants
const float PERSPECTIVE = 45;		// pov ratio
const float DF_PHYSICS_UPDATE_RATE = 100.0;  // per second
const float DF_FAST_AI_UPDATE_RATE =  40.0;  // per second
const float DF_SLOW_AI_UPDATE_FRACTION = 0.010;  // per fast AI update


