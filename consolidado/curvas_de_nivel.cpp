#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

void motion(int x, int y);
void noMotion(int x, int y);
void mouse(int button, int state, int x, int y);
void keyboardPress(unsigned char key, int x, int y);
void modifyImage();

int window1 = 0;
int window2 = 0;

int windowSizeX = 256;
int windowSizeY = 256;

int quad1PosX = 0;
int quad1PosY = 0;

int quad2PosX = windowSizeX*0.25;
int quad2PosY = windowSizeY*0.25;

int quad3PosX = windowSizeX*0.75;
int quad3PosY = windowSizeY*0.75;

int quad4PosX = 256;
int quad4PosY = 256;

bool quad1 = false;
bool quad2 = false;
bool quad3 = false;
bool quad4 = false;

PixelLab *img = NULL;
PixelLab *imgOriginal = NULL;

void displayConfig(void) {

	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer

	glClearColor (0.4, 0.4, 0.4, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, windowSizeX, 0.0, windowSizeY, -500.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  	//Ponto no canto inferior esquerdo
  	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(quad1PosX, quad1PosY, 0);
	glutSolidCube(10);
	glPopMatrix();

  glBegin(GL_LINES);
      glVertex2f (quad1PosX, quad1PosY);
      glVertex2f (quad2PosX, quad2PosY);
   glEnd();

   //Ponto movel da esquerda
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(quad2PosX, quad2PosY, 0);
	glutSolidCube(10);
	glPopMatrix();

  glBegin(GL_LINES);
      glVertex2f (quad2PosX, quad2PosY);
      glVertex2f (quad3PosX, quad3PosY);
   glEnd();

  //Ponto no canto superior direito
  glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(quad4PosX, quad4PosY, 0);
	glutSolidCube(10);
	glPopMatrix();

  glBegin(GL_LINES);
      glVertex2f (quad3PosX, quad3PosY);
      glVertex2f (quad4PosX, quad4PosY);
   glEnd();

  //Ponto movel da direita
  glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glTranslatef(quad3PosX, quad3PosY, 0);
	glutSolidCube(10);
	glPopMatrix();

	glutSwapBuffers ();  // Required when double buffer is initialized
	glutPostRedisplay();
}

void displayImage(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img->ViewImage();

	glutSwapBuffers();
  	glutPostRedisplay();
}

void changePixel(uByte &px) {

	int a = 1, b = 0;

	if (px >= quad1PosX && px < quad2PosX) {
		a = (quad2PosY - quad1PosY) / (quad2PosX - quad1PosX);
		b = quad1PosY - a * quad1PosX;
	}

	if (px >= quad2PosX && px < quad3PosX) {
		a = (quad3PosY - quad2PosY) / (quad3PosX - quad2PosX);
		b = quad2PosY - a * quad2PosX;
	}

	if (px >= quad3PosX && px < quad4PosX) {
		a = (quad4PosY - quad3PosY) / (quad4PosX - quad3PosX);
		b = quad3PosY - a * quad3PosX;
	}

	px = b + a * px;
}

void modifyImage() {

	uByte r, g, b;

	for(int x = 0; x < img->GetHeight(); x++) {
	   for(int y = 0; y < img->GetWidth(); y++) {
			 imgOriginal->GetRGB(x, y, r, g, b);
			 changePixel(r);
			 changePixel(g);
			 changePixel(b);
			 img->SetRGB(x, y, r, g, b);
	   }
	 }
	 // Update both windows
   glutPostWindowRedisplay(window2);
}

void init (void) {
	img = new PixelLab();
	img->Read("figs/lena.png");
	imgOriginal = new PixelLab();
	imgOriginal->Read("figs/lena.png");
}

// Main Function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	init ();

	// Create two windows
	window2 = glutCreateWindow("Display Window");
	window1 = glutCreateWindow("Control Window");

	//Janela da configuração
	glutSetWindow (window1);
	glutDisplayFunc(displayConfig);
	glutPositionWindow(100, 100);
	glutReshapeWindow(windowSizeX, windowSizeY);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboardPress);

	//Janela da imagem
	glutSetWindow (window2); // Change current window to 2
	glutDisplayFunc(displayImage);
	glutReshapeWindow(img->GetWidth(), img->GetHeight());
	glutPositionWindow(360, 100);
	glutKeyboardFunc(keyboardPress);
	glutMainLoop();

	return 0;
}

// Motion callback
void motion(int x, int y ) {
	if (quad1) {
  		y = windowSizeY - y;
  		quad1PosY = y;
  	}

  	if (quad2) {
  		y = windowSizeY - y;
  		quad2PosY = y;
  	}
	if (quad3) {
  		y = windowSizeY - y;
  		quad3PosY = y;
  	}

  	if (quad4) {
  		y = windowSizeY - y;
  		quad4PosY = y;
  	}
	modifyImage();
}

// Motion callback
void noMotion(int x, int y ) {
	// Cancel passive motion
}

// Mouse callback
void mouse(int button, int state, int x, int y) {
	y = windowSizeY - y;

	if ( button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) {
			if (x >= quad1PosX - 5 && x <= quad1PosX + 5 && y >= quad1PosY - 5 && y <= quad1PosY + 5) {
				     quad1PosY = y;
			   quad1 = true;
			}

			if (x >= quad2PosX - 5 && x <= quad2PosX + 5 && y >= quad2PosY - 5 && y <= quad2PosY + 5) {
				     quad2PosY = y;
			   quad2 = true;
			}

				if (x >= quad3PosX - 5 && x <= quad3PosX + 5 && y >= quad3PosY - 5 && y <= quad3PosY + 5) {
				     quad3PosY = y;
			   quad3 = true;
			}

			if (x >= quad4PosX - 5 && x <= quad4PosX + 5 && y >= quad4PosY - 5 && y <= quad4PosY + 5) {
				     quad4PosY = y;
			   quad4 = true;
			}
		} else {
      		quad1 = quad2 = quad3 = quad4 = false;
			printf("\nBotao esquerdo solto na posicao [%d, %d].", x, y);  // GLUT_UP
    	}
	} else
		printf("\nOutro botao do mouse pressionado.");
}


// Keyboard callback
void keyboardPress(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
		break;
		default:
			printf("\nUma tecla não mapeada foi pressionada");
		break;
	}
}
