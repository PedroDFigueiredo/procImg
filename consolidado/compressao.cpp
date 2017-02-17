#include <ctype.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <list>
#include "lzw.cpp"
#include "rle.cpp"

using namespace std;

int window1 = 0;

PixelLab *img1 = NULL;

int UI_width = 0;

void displayImage(void);
void add(int);
void idle();
void Huffman(int);
void LZW(int);
void RLE(int);

void init (void) {
	img1 = new PixelLab();
	img1->Read("figs/lena.png");
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
	window1 = glutCreateWindow("Compactação");

	glutSetWindow (window1);
	glutDisplayFunc(displayImage);
	glutPositionWindow(100, 356);

	GLUI *glui = GLUI_Master.create_glui_subwindow(window1, GLUI_SUBWINDOW_RIGHT);

  	//GLUI_Master.set_glutReshapeFunc(reshape);

  	GLUI_Panel *io_panel = glui->add_panel( (char *) "Compressão" );
  	glui->add_column_to_panel(io_panel, false);

   	GLUI_Button *b1 = glui->add_button_to_panel(io_panel, (char*) "Huffman", 0, Huffman);
  	b1->set_w(50);

  	GLUI_Button *b2 = glui->add_button_to_panel(io_panel, (char*) "LZW", 0, LZW);
  	b1->set_w(50);

  	GLUI_Button *b3 = glui->add_button_to_panel(io_panel, (char*) "RLE", 0, RLE);
  	b3->set_w(50);

  	computeUIWidth();
  	glutReshapeWindow(img1->GetWidth() + UI_width, img1->GetHeight());

  	glui->set_main_gfx_window(window1);

	glutMainLoop();

	return 0;
}

void displayImage(void) {
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer
	img1->ViewImage();

	//glOrtho(0, img1->GetWidth() - 1, 0, img1->GetHeight() - 1, -1, 1);

	glutSwapBuffers();
 	glutPostRedisplay();
}

void Huffman(int op) {

}

void LZW(int i) {
	vector<string> dicR;
	vector<string> dicG;
	vector<string> dicB;

	vector<short int> sequenceR;
	vector<short int> sequenceG;
	vector<short int> sequenceB;
	lzw(img1, dicR, dicG, dicB, sequenceR, sequenceG, sequenceB);
}

void RLE(int i) {
	rle(img1);
}

void idle() {
	if ( glutGetWindow() != window1)
		glutSetWindow(window1);
	glutPostRedisplay();
}