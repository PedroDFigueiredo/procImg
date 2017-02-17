#include "pixelLab.h"
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glui.h>

using namespace std;

int window1 = 0;
int window2 = 0;
int window3 = 0;

int op = 0;

PixelLab *img1 = NULL, *img2 = NULL, *img3 = NULL;

int windth, height, UI_width = 0;

int posX = 128;

int histo[256];

double _T = 0.01, T = 0;

int segmentacaoFlag = 1;
char *input_img;

void displayImage1(void);
void displayImage2(void);
void displayImage3(void);
void modifyImage(int);
void specialKeyboard(int key, int x, int y);
void applyThresholding(PixelLab *image, float T);
//void findT(int);

void init (void) {
	img1 = new PixelLab();
	img1->Read(input_img);

	img2 = new PixelLab();
	img2->CreateImage(img1->GetWidth(), img1->GetHeight());
}

void reshape( int x, int y ) {
  glutPostRedisplay();
}

void computeUIWidth() {
   int aux, vw;
   GLUI_Master.get_viewport_area(&aux, &aux, &vw, &aux);
   UI_width = vw;
}

// Main Function
int main(int argc, char** argv) {
   input_img = argv[1];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	init ();

	// Create two windows
	window1 = glutCreateWindow("Control Image 1");
  	window2 = glutCreateWindow("Control Image 2");
  	window3 = glutCreateWindow("Control Operation");

	//Janela da imagem
	glutSetWindow (window1);
	glutDisplayFunc(displayImage1);
	glutReshapeWindow(img1->GetWidth(), img1->GetHeight());
	glutPositionWindow(10, 356);

  	//Janela da Histograma
	glutSetWindow (window2);
	glOrtho(0.0, 255.0, 0, 3000.0, 0.0, 255.0);
	glutDisplayFunc(displayImage2);
	glutSpecialFunc(specialKeyboard);
	glutPositionWindow(img1->GetWidth()+30, 356);
	glutReshapeWindow(256, 300);

  	//Janela da imagem
	glutSetWindow (window3); // Change current window to 2
	glutDisplayFunc(displayImage3);
	glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
	glutPositionWindow(img1->GetWidth() + 310, 356);

	glutMainLoop();

	return 0;
}

void displayImage1(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img1->ViewImage();

	glutSwapBuffers();
 	//glutPostRedisplay();
}

void displayImage2(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	glClearColor (1.0, 1.0, 1.0, 0.0);

	for(int x = 0; x < 256; x++) {
		histo[x] = 0;
	}

	int pixelVal = 0;

	for(int x = 0; x < img1->GetWidth(); x++) {
		for(int y = 0; y < img1->GetHeight(); y++) {
			pixelVal = img1->GetGrayValue(x, y);
			histo[pixelVal]++;
		}
	}

	glColor3f (1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f (posX, 0);
	glVertex2f (posX, 3000);
	glEnd();

	glColor3f (0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i < 256; i++) {
		glVertex2f (i+1, 0);
	    glVertex2f (i+1, histo[i]);
	}
	glEnd();

	applyThresholding(img1, histo[posX]);

	glutSwapBuffers();
  	glutPostRedisplay();
}

void displayImage3(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	glutSwapBuffers();
  	//glutPostRedisplay();
}

double abs(double value) {
	double v = value * (-1 * (value < 0) + 1 * (value >= 0));
	return v;
}

void specialKeyboard(int key, int x, int y) {
   switch (key) {
      case GLUT_KEY_RIGHT:
      	if (posX < 254)
        	posX++;
        break;

      case GLUT_KEY_LEFT:
      	if (posX > 0)
        	posX--;
        break;

      default:
         break;
   }
}

void applyThresholding(PixelLab *image, float T) {
	for (int i = 0; i < image->GetHeight(); i++) {
		for (int j = 0; j < image->GetWidth(); j++) {
			if (image->GetGrayValue(i, j) > T) {
				img2->SetRGB(i, j, (uByte)0, (uByte)0, (uByte)0);
			} else {
				img2->SetRGB(i, j, (uByte)255, (uByte)255, (uByte)255);
			}
		}
	}
	glutPostWindowRedisplay(window3);
}