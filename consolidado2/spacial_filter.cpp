#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glui.h>

int window1 = 0;
int window2 = 0;
int window3 = 0;

int op = 0;

PixelLab *img1 = NULL, *img2 = NULL, *img3 = NULL;

int windth, height, UI_width = 0;

char *input_img;

void displayImage1(void);
void displayImage2(void);
void displayImage3(void);
void modifyImage(int);
void mouse(int, int, int, int, int, int);
void mouse1(int, int, int, int);
void mouse2(int, int, int, int);
void keyboardPress(unsigned char, int, int);
void mouse(int, int, int, int);

void init (void) {
	img1 = new PixelLab();
	img1->Read(input_img);

  img2 = new PixelLab();
	img2->Read("figs/merge.example.png");

	img3 = new PixelLab();
	img3->Read(input_img);
	int w, h;

	w = img1->GetWidth() < img2->GetWidth() ? img2->GetWidth() : img1->GetWidth();
	h = img1->GetHeight() < img2->GetHeight() ? img2->GetHeight() : img1->GetHeight();

	img3->SetWidth(w);
	img3->SetHeight(h);
}

void computeUIWidth() {
   int aux, vw;
   GLUI_Master.get_viewport_area(&aux, &aux, &vw, &aux);
   UI_width = img2->GetWidth()-vw;
}

void idle() {
   if ( glutGetWindow() != window2)
      glutSetWindow(window2);
   glutPostRedisplay();
}

void reshape( int x, int y ) {
  glutPostRedisplay();
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
	glutSetWindow (window1); //Change current window to 2
	glutDisplayFunc(displayImage1);
	glutReshapeWindow(img1->GetWidth(), img1->GetHeight());
	glutPositionWindow(100, 356);
	glutMouseFunc(mouse1);

  //Janela da imagem
	glutSetWindow (window2); //Change current window to 2
	glutDisplayFunc(displayImage2);
	//glutReshapeWindow(img2->GetWidth(), img2->GetHeight());
	glutPositionWindow(300, 356);
	glutMouseFunc(mouse2);

	GLUI *glui = GLUI_Master.create_glui_subwindow(window2, GLUI_SUBWINDOW_RIGHT );

  GLUI_Master.set_glutReshapeFunc(reshape);

  GLUI_Panel *io_panel = glui->add_panel( (char *) "Operações" );
  glui->add_column_to_panel( io_panel, false);
  GLUI_Button *b1 = glui->add_button_to_panel(io_panel, (char*) "Ou Lógico", 0, modifyImage);
  GLUI_Button *b2 = glui->add_button_to_panel(io_panel, (char*) "E Lógico", 1, modifyImage);
	GLUI_Button *b3 = glui->add_button_to_panel(io_panel, (char*) "XOR Lógico", 2, modifyImage);
  b1->set_w(50);
  b2->set_w(50);
	b3->set_w(50);

  glui->set_main_gfx_window(window2);
  GLUI_Master.set_glutIdleFunc(idle);
  computeUIWidth(); // Compute the size of the user interface
  glutReshapeWindow(img2->GetWidth() + UI_width, img2->GetHeight());

  //Janela da imagem
	glutSetWindow (window3); // Change current window to 2
	glutDisplayFunc(displayImage3);
	glutReshapeWindow(img3->GetWidth(), img3->GetHeight());
	glutPositionWindow(300, 356);
	glutMainLoop();

	return 0;
}

void displayImage1(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img1->ViewImage();

	glutSwapBuffers();
  glutPostRedisplay();
}

void displayImage2(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img2->ViewImage();

	glutSwapBuffers();
  glutPostRedisplay();
}

void displayImage3(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img3->ViewImage();

	glutSwapBuffers();
  glutPostRedisplay();
}

void modifyImage(int op) {
	uByte r1, r2, g1, g2, b1, b2;
	for(int x = 0; x < img3->GetWidth(); x++) {
		for(int y = 0; y < img3->GetHeight(); y++) {

			if (x < img1->GetWidth() && y < img1->GetHeight()) {
				img1->GetRGB(x, y, r1, g1, b1);
			}else
				r1 = 255; g1 = 255; b1 = 255;

			if (x < img2->GetWidth() && y < img2->GetHeight()) {
				img2->GetRGB(x, y, r2, g2, b2);
			}else
				r2 = 255; g2 = 255; b2 = 255;

			switch(op){
				case 0:
					img3->SetRGB(x, y, r1 | r2, g1 | g2, b1 | b2);
					break;
				case 1:
					img3->SetRGB(x, y, r1 & r2, g1 & g2, b1 & b2);
					break;
				case 2:
					img3->SetRGB(x, y, r1 ^ r2, g1 ^ g2, b1 ^ b2);
					break;
				default:
					break;
			}
		}
	}
	// Update both windows
  glutPostWindowRedisplay(window3);
}

// Keyboard callback
void keyboardPress(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
			break;
		case '1':
			op = 0;
			break;
		case '2':
			op = 1;
			break;
		case '3':
			op = 2;
			break;
		default:
			printf("\nUma tecla não mapeada foi pressionada");
			break;
	}
	modifyImage(op);
}

void mouse1(int button, int state, int x, int y) {
	mouse(button, state, x, y, img1->GetWidth(), img1->GetHeight());
}

void mouse2(int button, int state, int x, int y) {
	mouse(button, state, x, y, img2->GetWidth(), img2->GetHeight());
}

// Mouse callback
void mouse(int button, int state, int x, int y, int width, int height) {
	y = img2->GetHeight() - y;

	if (button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) {

		} else {
      printf("\nBotao esquerdo solto na posicao [%d, %d].", x, y);  // GLUT_UP
    }
	} else
		printf("\nOutro botao do mouse pressionado.");
}

// Mouse callback
void mouse(int button, int state, int x, int y) {
	printf("\nbotao do mouse pressionado.");
}
