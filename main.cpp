#include "main.h"


//global variables
SpriteFont font;	//handle for font
EyeCam eye = EyeCam(500, 30, 0);	//eye starting position
SlimeSpawner slimePop;	//handle for all slime


float physDelta = 1000/DF_PHYSICS_UPDATE_RATE;	//ms per update
long physLag = 0;	//remaining ms for physics updates
float aiDelta = 1000/DF_FAST_AI_UPDATE_RATE;		//ms per update
long aiLag = 0;		//remaining ms for ai updates
float slowai = DF_SLOW_AI_UPDATE_FRACTION;		//fraction of fast ai
float frameDelta;

//difference used to calculate delta time
long prevTime; //value set before frame update
long curTime; //value set after frame update

bool keyPressed[256];		//keyboard state array
bool specialKeyPressed[2];	//special key state array
float map[100][100];		//height map array

DisplayList seaFloor; 


////////////////MAIN FUNCTION////////////////
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	// single is default
	glutInitWindowSize(600, 500);		//window size
	glutInitWindowPosition(300, 200);	//window position
	glutCreateWindow("EyeCam");		//window title

	glutReshapeFunc(reshape);			//window reshape function
	glutIdleFunc(update);				//idle function
	
	glutKeyboardFunc(keyboard);			//key press function
	glutKeyboardUpFunc(keyboardUp);		//key release function
	glutSpecialFunc(specialKey);		//special key press function
	glutSpecialUpFunc(specialKeyUp);	//special key release function
	
	glutDisplayFunc(display);			//display function
	init();		//initialise game state
	glutMainLoop();
	return 1;
}
////////////////INIT FUNCTION////////////////
void init(void){	
	//eye.eyeModel.load("eyemarine.obj");
	//readHeightMap();
	font.load("Font.bmp");	//load font img

	float seaColor = (eye.yPos)/1000;	//relative yPos used for background color
	glClearColor(0.0, seaColor, seaColor, 1.0);		// background color

	for (int i = 0; i < 256; i++)	//initialise keyboard array
		keyPressed[i] = false;
	specialKeyPressed[0] = false; specialKeyPressed[1] = false;

	//initialise the random number of slime, the rest are ignored
	slimePop = SlimeSpawner();
	prevTime = glutGet(GLUT_ELAPSED_TIME);
	
	glEnable(GL_DEPTH_TEST);

}


//////////////////////////update game state///////////////////////////////
void update(){	
	glutPostRedisplay();	//update display	
	//sleep(0.01); //wait for 0.01 seconds

	//respond to key presses
	keyFunctions();	

	//cout << "update graphics" << endl; //debug message

	curTime = glutGet(GLUT_ELAPSED_TIME); //determine approx delta time
	frameDelta = curTime - prevTime;
	physLag += frameDelta; //alot time for physics updates
	aiLag += frameDelta; //alot time for ai updates

	//* //perform updates based on time available
	while (physLag > physDelta || aiLag > aiDelta){	
		if(physLag - physDelta > aiLag - aiDelta){
			eye.physUpdate(physDelta);
			slimePop.physUpdate(physDelta);
			physLag -= physDelta;
			//cout << "update physics" << endl; //debug message
		}else{
			aiLag -= aiDelta;
			//cout << "update ai" << endl; //debug message
		}
	}//*/
	prevTime = curTime;	
}


///////////////////////////display///////////////////////////
void display(void){
	if (slimePop.win()) winScreen();		//check win condition
	//relative yPos used for background color
	//changeSeaColor();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();	// initialize the selected matrix

	//look at point follows behind eye
	eye.camera();

	//draw function
	drawSkyAndFloor();
	printMaxim(eye.xPos, eye.yPos-30, eye.zPos+50);
	slimePop.drawSlime();
	eye.draw(); 
	drawText();

	glFlush();
	glutSwapBuffers();		// display it, long buffering
}


//reshape window update
void reshape(int w , int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(PERSPECTIVE, (long)w / (long)h, 0.1, 1000.0);
}

//user input
void keyFunctions(){
	//move eye toward the surface
	if (keyPressed['W'] || keyPressed['w'])	eye.yPos += 1;
	//move eye toward the bottom			
	if (keyPressed['S'] || keyPressed['s']) eye.yPos -= 1;
	//turn right
	if (keyPressed['A'] || keyPressed['a'])	eye.xPos += 1; 
	//turn left
	if (keyPressed['D'] || keyPressed['d']) eye.xPos -= 1;

	if (specialKeyPressed[0])	//move forward
		eye.engineGo(frameDelta);
	if (specialKeyPressed[1])	//move backward
		eye.engineGo(-frameDelta);

	//increase slime count
	if (keyPressed['T'] || keyPressed['t'])	slimePop.increaseSlime();
	//decrease slime count
	if (keyPressed['G'] || keyPressed['g']) slimePop.decreaseSlime();
	//increase physic update rate
	if (keyPressed['Y'] || keyPressed['y'])	physDelta += 1;
	//decrease physic update rate
	if (keyPressed['H'] || keyPressed['h']) physDelta -= 1;
	//increase fast ai update rate
	if (keyPressed['U'] || keyPressed['u'])	aiDelta += 1;
	//decrease fast ai update rate
	if (keyPressed['J'] || keyPressed['j']) aiDelta -= 1;
	//increase slow ai update rate
	if (keyPressed['I'] || keyPressed['i'])	slowai += 0.001;
	//decrease slow ai update rates
	if (keyPressed['K'] || keyPressed['k']) slowai -= 0.001;
	if (keyPressed[27]) exit(0); // normal exit
}


void drawText(){
	int textR; int textG; int textB;
	stringstream ss;
	SpriteFont::setUp2DView(600, 500);
		int fontThreshold = 500;
		if (eye.yPos > fontThreshold){ 
			textR = 0; textG = 0;  textB = 0;
		} else {textR = 255; textG = 255;  textB = 255;}

		//display rates
		ss << "Physics rate: " << 1000/physDelta;
		font.draw(ss.str(), 15, 15, textR, textG, textB);
		ss.str(""); //reset string stream
		ss << "Fast AI rate: " << 1000/aiDelta;
		font.draw(ss.str(), 15, 40, textR, textG, textB);
		ss.str(""); //reset string stream
		ss << "Slow AI fraction: " << slowai;
		font.draw(ss.str(), 15, 65, textR, textG, textB);
		ss.str(""); //reset string stream
		ss << "Frame rate: " << 1000/frameDelta;
		font.draw(ss.str(), 15, 90, textR, textG, textB);
		ss.str(""); //reset string stream

		//print position //reverse x values
		ss << "(" << unsigned(1000-eye.xPos) << ", " << eye.yPos << ", " << eye.zPos << ")";
		//display remaining slime	
		font.draw(ss.str(), 15, 450, textR, textG, textB);
		ss.str(""); //reset string stream
		ss << "Vertical: " << eye.speed.y;
		font.draw(ss.str(), 15, 425, textR, textG, textB);
		ss.str(""); //reset string stream

		//display score
		ss << "Slime Count: " << slimePop.slimeCount - slimePop.slimeCaught;
		font.draw(ss.str(), 430, 10, textR, textG, textB);
		ss.str(""); //reset string stream

		ss << "Points: " << slimePop.slimeCaught;
		font.draw(ss.str(), 470, 35, textR, textG, textB);
		ss.str(""); //reset string stream

	SpriteFont::unsetUp2DView();
}

void changeSeaColor(){
	float seaColor = (eye.yPos)/1000;
	//prevent background from getting to dark or bright
	if (seaColor > 0.7) seaColor = 0.7;	
	if (seaColor < 0.1) seaColor = 0.1;
	glClearColor(0.0, seaColor, seaColor, 1.0);
}

void printMaxim(float xPos, float yPos, float zPos){
glPushMatrix();
		glColor3d(1, 1, 1);	//darken the texture
		glEnable(GL_TEXTURE_2D);
		TextureManager::activate("maxim.bmp");
		glBegin(GL_QUADS);
			
			glTexCoord2d(0, 0);  glVertex3d(xPos-5, yPos, zPos);
			glTexCoord2d(-1, 0);  glVertex3d(xPos+5, yPos, zPos);
			glTexCoord2d(-1, 1);  glVertex3d(xPos+5, yPos+8.5, zPos+5);
			glTexCoord2d(0, 1);  glVertex3d(xPos-5, yPos+8.5, zPos+5);		
		
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawSkyAndFloor(){
		//draw the sky
		glBegin(GL_POLYGON);
			glColor3f(0.5, 1.0, 1.0);
			glVertex3f(1000.0, -5, 1000.0);
			glVertex3f(0, -5, 1000.0);
			glVertex3f(0, 500.0, 1000.0);
			glVertex3f(1000, 500.0, 1000.0);
		glEnd();
		//*/draw the ocean bottom
		glBegin(GL_POLYGON);
			glColor3f(0, 0.6, 0);
			glVertex3f(0.0, -5.0, 0.0);
			glVertex3f(1000, -5.0, 0.0);
			glVertex3f(1000, -5.0, 1000);
			glVertex3f(0.0, -5.0, 1000);
		glEnd();//*/
		//seaFloor.draw();
}




void winScreen(){
		//win response
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();	// initialize the selected matrix
		//print win message
		SpriteFont::setUp2DView(600, 500); 
			font.draw("You Win!", 260, 220, 255, 255, 255);	
		SpriteFont::unsetUp2DView();
		glFlush();
		glutSwapBuffers();	
		sleep(2);
		exit(0);		//close game
}



void keyboard(unsigned char key, int x, int y){
	keyPressed[key] = true;
}
void keyboardUp(unsigned char key, int x, int y)
{
	keyPressed[key] = false;
}

void specialKey(int special_key, int x, int y){
	if (special_key == GLUT_KEY_UP)	
		specialKeyPressed[0] = true;
	if (special_key == GLUT_KEY_DOWN)
		specialKeyPressed[1] = true;
}
void specialKeyUp(int special_key, int x, int y){
	if (special_key == GLUT_KEY_UP)	//move forward
		specialKeyPressed[0] = false;
	if (special_key == GLUT_KEY_DOWN)
		specialKeyPressed[1] = false;
}


void readHeightMap(){
	ifstream input;
	int r; int c;
	// read from text file
	input.open("Heightmap.txt", ios::in | ios::binary);
	if(input.is_open()){
		input >> r; input >> c; //read in array size
		for (int i = 0; i < r; i++){
			for (int j = 0; j < c; j++){
				input >> map[i][j];
			}
		}
	}	input.close();

	int gridSize = 50;
	int cel = 1000/gridSize;

	seaFloor.begin();	//start display list
	glPushMatrix();
		glColor3d(0.02, 0.04, 0.04);	//darken the texture
		glEnable(GL_TEXTURE_2D);
		TextureManager::activate("sand.bmp");
		glBegin(GL_QUADS);
		for (int i = 0; i < gridSize; i++){
			for (int j = 0; j < gridSize; j++){
				if (i+1 == gridSize) r = 0;
				else r = i+1;	//wrap around 
				if (j+1 == gridSize) c = 0;
				else c = j+1;	//wrap around

				 glTexCoord2d(0, 0);  glVertex3d(i*cel, map[i][j]-50, j*cel);
				 glTexCoord2d(1, 0);  glVertex3d(i*cel, map[i][c]-50, (j+1)*cel);
				 glTexCoord2d(1, 1);  glVertex3d((i+1)*cel, map[r][c]-50, (j+1)*cel);
				 glTexCoord2d(0, 1);  glVertex3d((i+1)*cel, map[r][j]-50, j*cel);		}
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	seaFloor.end();
}