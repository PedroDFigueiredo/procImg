#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <ctype.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <vector>

using namespace std;

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct _Seed {
	int x;
	int y;

	uByte min;
	uByte max;

	uByte r;
	uByte g;
	uByte b;
} Seed ;

int posX, posY;

vector<Seed> seeds;

int window1 = 0;

PixelLab *img1 = NULL;

int UI_width = 0;

GLUI_EditText *editMax;
GLUI_EditText *editMin;

GLUI_EditText *editR;
GLUI_EditText *editG;
GLUI_EditText *editB;

char _max[512];
char _min[512];

char R[512];
char G[512];
char B[512];

void displayImage(void);
void add(int op);
void run(int op);
void idle();
void mouse(int button, int state, int x, int y);
void execute(Seed);

void init (void) {
	img1 = new PixelLab();
	img1->Read("figs/CT.lungs.png");
}

void computeUIWidth() {
   int aux, vw;
   GLUI_Master.get_viewport_area(&aux, &aux, &vw, &aux);
   UI_width = vw;
}

// Main Function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	init ();

	// Create two windows
	window1 = glutCreateWindow("Segmentação II");

	computeUIWidth();
	glutSetWindow (window1);
	glutDisplayFunc(displayImage);
	glutMouseFunc(mouse);
	glutReshapeWindow(img1->GetWidth() + UI_width, img1->GetHeight());
	glutPositionWindow(100, 356);

	GLUI *glui = GLUI_Master.create_glui_subwindow(window1, GLUI_SUBWINDOW_RIGHT);

  	//GLUI_Master.set_glutReshapeFunc(reshape);

  	GLUI_Panel *io_panel = glui->add_panel( (char *) "Valores" );
  	glui->add_column_to_panel(io_panel, false);
  	
  	/*Threshold 1*/
	editMax = glui->add_edittext_to_panel (io_panel, (char *) "Max", GLUI_EDITTEXT_TEXT, _max);
   	editMax->set_w(200);
   	/*Threshold 1*/

   	/*Threshold 2*/
	editMin = glui->add_edittext_to_panel (io_panel, (char *) "Min", GLUI_EDITTEXT_TEXT, _min);
   	editMin->set_w(200);
   	/*Threshold 2*/

   	/*Cor 1*/
   	editR = glui->add_edittext_to_panel (io_panel, (char *) "R1", GLUI_EDITTEXT_TEXT, R);
   	editR->set_w(200);

   	editG = glui->add_edittext_to_panel (io_panel, (char *) "G1", GLUI_EDITTEXT_TEXT, G);
   	editG->set_w(200);

   	editB = glui->add_edittext_to_panel (io_panel, (char *) "B1", GLUI_EDITTEXT_TEXT, B);
   	editB->set_w(200);
   	/*Cor 1*/

   	GLUI_Button *b1 = glui->add_button_to_panel(io_panel, (char*) "Add", 0, add);
  	b1->set_w(50);

  	GLUI_Button *b2 = glui->add_button_to_panel(io_panel, (char*) "Run", 0, run);
  	b2->set_w(50);

  	glui->set_main_gfx_window(window1);

	glutMainLoop();

	return 0;
}

void displayImage(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img1->ViewImage();

	glOrtho(0, img1->GetWidth() - 1, 0, img1->GetHeight() - 1, -1, 1);

	glutSwapBuffers();
 	glutPostRedisplay();
}

void add(int op) {

	Seed seed;
	seed.x = posX;
	seed.y = posY;

	seed.max = (uByte)atoi(_max);
	seed.min = (uByte)atoi(_min);

	seed.r = (uByte)atoi(R);
	seed.g = (uByte)atoi(G);
	seed.b = (uByte)atoi(B);

	seeds.push_back(seed);

	glutPostWindowRedisplay(window1);
}

void run(int op) {

	for (int i = 0; i < seeds.size(); ++i) {
		Seed s = seeds[i];
		cout << "(" << s.x << ", " << s.y << ")\n";
		execute(s);
	}

	glutPostWindowRedisplay(window1);
}

bool find(Seed* seed, bool** visitedMat) {
	if (seed->x-1 >= 0 && !visitedMat[seed->x-1][seed->y]) {
		seed->x -= 1;
		return true;
	} else if (seed->x+1 < img1->GetWidth() && !visitedMat[seed->x+1][seed->y]) {
		seed->x += 1;
		return true;
	} else if (seed->y-1 >= 0 && !visitedMat[seed->x][seed->y-1]) {
		seed->y -= 1;
		return true;
	} else if (seed->y+1 < img1->GetHeight() && !visitedMat[seed->x][seed->y+1]) {
		seed->y += 1;
		return true;
	} else if (seed->x+1 < img1->GetWidth() && seed->y+1 < img1->GetHeight() && !visitedMat[seed->x+1][seed->y+1]) {
		seed->x += 1;
		seed->y += 1;
		return true;
	} else if (seed->x-1 >= 0 && seed->y-1 >= 0 && !visitedMat[seed->x-1][seed->y-1]) {
		seed->x -= 1;
		seed->y -= 1;
		return true;
	} else if (seed->x-1 >= 0 && seed->y+1 < img1->GetHeight() && !visitedMat[seed->x-1][seed->y+1]) {
		seed->x -= 1;
		seed->y += 1;
		return true;
	} else if (seed->x+1 < img1->GetWidth() && seed->y-1 >= 0 && !visitedMat[seed->x+1][seed->y-1]) {
		seed->x += 1;
		seed->y -= 1;
		return true;
	}
	return false;
}

void execute(Seed seed) {
	vector<Seed> stack;
	bool** visited = (bool**) malloc(sizeof(bool*) * img1->GetHeight());

	for (int i = 0; i < img1->GetHeight(); i++) {
		visited[i] = (bool*) malloc(sizeof(bool) * img1->GetWidth());
	}

	for (int i = 0; i < img1->GetHeight(); i++) {
		for (int j = 0; j < img1->GetHeight(); j++) {
			visited[i][j] = false;
		}
	}

	while(true) {
		if(!visited[seed.x][seed.y]) {
			if (img1->GetGrayValue(seed.x, seed.y) >= seed.min && img1->GetGrayValue(seed.x, seed.y) <= seed.max) {
				img1->SetRGB(seed.x, seed.y, seed.r, seed.g, seed.b);
				stack.push_back(seed); // insere "v" na pilha
			} else {
				if (stack.size() > 0) {
					stack.pop_back();
					if (stack.size() > 0) {
						seed = stack.back();
					}
				}
				
			}
			visited[seed.x][seed.y] = true; // marca como visitado
			
		}

		if (!find(&seed, visited)) {

			if (stack.size() > 0)
				stack.pop_back();

			if (stack.size() == 0)
				break;

			seed = stack.back();
		}

	}
}

// Mouse callback
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x > 0 && x < img1->GetWidth() && y > 0 && y < img1->GetHeight()) {
		posX = x;
		posY = y;
   	}
}

void idle() {
	if ( glutGetWindow() != window1)
		glutSetWindow(window1);
	glutPostRedisplay();
}